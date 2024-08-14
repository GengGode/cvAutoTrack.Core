#pragma once
#include <map>
#include <mutex>
#include <memory>
#include <thread>
#include <functional>
#include <opencv2/core/mat.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_tex_inspect/imgui_tex_inspect.h"
#include "imgui_implot/implot.h"
#include "imgui_implot/implot_internal.h"


class image_inspect_pool
{
public:
    using tex_inspect_context = ImGuiTexInspect::Context*;
    using context_ptr = std::shared_ptr<tex_inspect_context>;
    struct rect_info
    {
        double x_min;
        double y_min;
        double x_max;
        double y_max;
    };
    struct context_info
	{
        tex_inspect_context context = 0;
		ImTextureID texture = 0;
		cv::Mat     mat;
        bool        is_open = false;
        bool        is_need_rebind = false;
        std::map<int, rect_info> roi_map;
	};
    std::map<std::string, context_info> context_map;
    std::mutex                          mutex;

public:
    std::function<void(cv::Mat&,ImTextureID&)> mat_to_texture;
    std::function<void(ImTextureID)> release_texture;
public:
    void render()
    {
        std::unique_lock lock(mutex);

        ImGui::Begin("图片池");
        ImGui::BeginChild("图片列表");
        std::string select_key = "";
        for (auto& [key, value] : context_map)
		{
			ImGui::PushID(key.c_str());
            ImGui::BeginGroup();
            ImGui::Image(value.texture, ImVec2(100, 100));
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text(key.c_str());
                ImGui::EndTooltip();
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            if (ImGui::Button("X"))
            {
                select_key = key;
            }
            if (value.is_open)
            {
                ImGui::SameLine();
                if (ImGui::Button("隐藏"))
                {
                    value.is_open = false;
                }
            }
            else
            {
                ImGui::SameLine();
                if (ImGui::Button("显示"))
                {
                    value.is_open = true;
                }
            }
            ImGui::Text("(%d,%d)", value.mat.cols, value.mat.rows);
            ImGui::EndGroup();
            ImGui::PopID();
		}
        ImGui::EndChild();
        ImGui::End();

        if(select_key != "")
		{
			sync_remove(select_key);
		}

        for (auto& [key, value] : context_map)
        {
            auto widget_name = "Viewer : " + key;
            auto inspector_name = "Inspector : " + key;
            if (value.is_open)
            {
                ImGui::Begin(widget_name.c_str(), &value.is_open);
                auto size = ImGui::GetContentRegionAvail();
                auto image_size = ImVec2(value.mat.cols, value.mat.rows);
                if (ImGui::Button("+"))
                {
                    value.roi_map.insert_or_assign(value.roi_map.size(), rect_info{ 0,0,static_cast<double>(value.mat.cols),static_cast<double>(value.mat.rows) });
                }
                if (ImPlot::BeginPlot(widget_name.c_str(),size - ImVec2(0,30), ImPlotFlags_NoLegend | ImPlotFlags_NoTitle | ImPlotFlags_Equal)) {
                    ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
                    ImPlot::SetupAxesLimits(0, image_size.x, 0, image_size.y);
                    ImPlot::SetupAxis(ImAxis_Y1, 0, ImPlotAxisFlags_Invert);
                    ImPlot::PlotImage(inspector_name.c_str(), value.texture, {}, image_size, {}, { 1, -1 });
                    for (auto& [rect_id, rect] : value.roi_map)
                    {
                        ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1, 0, 1, 0.1));
						ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 0, 1, 1));
						ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2.0f);
						ImPlot::PushStyleVar(ImPlotStyleVar_Marker, 0);
						ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 0);
						ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, 0);
						ImPlot::DragRect(rect_id, &rect.x_min, &rect.y_min, &rect.x_max, &rect.y_max, ImVec4(1, 0, 1, 1));
						ImPlot::PopStyleVar(4);
						ImPlot::PopStyleColor(2);
                    }
                    //ImPlot::DragRect(0, &rect.X.Min, &rect.Y.Min, &rect.X.Max, &rect.Y.Max, ImVec4(1, 0, 1, 1), flags);
                    ImPlot::EndPlot();
                }
                ImGui::End();
            }
        }
	}
	void bind_texture()
	{
        std::unique_lock lock(mutex);
		for (auto& [key, value] : context_map)
		{
            if (value.is_need_rebind)
            {
                mat_to_texture(value.mat, value.texture);
                value.is_need_rebind = false;
            }
		}
        for(auto it = context_map.begin(); it != context_map.end();)
		{
            auto& [key, value] = *it;
            for (auto& [rect_id, rect] : value.roi_map)
            {
                auto sub_key = std::format("{} roi_{}", key, rect_id);
                auto roi_rect = cv::Rect(rect.x_min, rect.y_min, rect.x_max - rect.x_min, rect.y_max - rect.y_min);
                roi_rect = roi_rect&cv::Rect(0, 0, value.mat.cols, value.mat.rows);
                if (roi_rect.width <= 0 || roi_rect.height <= 0)
                    continue;
                auto roi_mat = value.mat(roi_rect);
                sync_update(sub_key, roi_mat);
            }
            it++;
		}
	}
    void task_process()
    {
        std::unique_lock lock(list_mutex);
        for (auto& [key, mat] : update_list)
        {
            if (context_map.find(key) == context_map.end())
            {
                context_info info;
                info.is_need_rebind = true;
                info.mat = mat;//.clone();
                info.context = ImGuiTexInspect::CreateContext();
                context_map.insert_or_assign(key, info);
                continue;
            }
            auto& value = context_map[key];
            value.mat = mat;//.clone();
            value.is_need_rebind = true;
        }
        for (auto& key : remove_list)
        {
            if (context_map.find(key) == context_map.end())
                continue;
            auto& info = context_map[key];
            release_texture(info.texture);
            ImGuiTexInspect::DestroyContext(info.context);
            context_map.erase(key);
        }
        update_list.clear();
        remove_list.clear();
    }
    std::list<std::tuple<std::string,cv::Mat>>          update_list;
    std::list<std::string>                              remove_list;
    std::mutex                                          list_mutex;
public:
    void sync_update(const std::string key, const cv::Mat& mat)
    {
        std::unique_lock lock(list_mutex);
        update_list.push_back({ key, mat.clone() });
    }
    void sync_remove(std::string key)
    {
        std::unique_lock lock(list_mutex);
        remove_list.push_back(key);
    }
    void async_update(std::string key, cv::Mat& mat)
	{
        std::jthread async = std::jthread([this, key, mat]() {
            sync_update(key, mat.clone());
        });
        async.detach();
	}
    void async_remove(std::string key)
    {
        std::jthread async = std::jthread([this, key]() {
            sync_remove(key);
        });
        async.detach();
    }
};
