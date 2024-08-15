#include "debugger.hpp"

#include <opencv2/imgproc.hpp>

static int gl_texture_color_format = GL_RGBA;

static auto bind_texture=[](cv::Mat& mat, ImTextureID& texture_id, bool need_to_rgba)
{
    if (mat.empty())
        return;
    if (texture_id == nullptr)
    {
        GLuint new_texture_id = 0;
        glGenTextures(1, &new_texture_id);
        texture_id = reinterpret_cast<ImTextureID>(static_cast<int64_t>(new_texture_id));
    }
    if(need_to_rgba)
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2RGBA);
    glBindTexture(GL_TEXTURE_2D, texture_cast(texture_id));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_texture_color_format, mat.cols, mat.rows, 0, gl_texture_color_format, GL_UNSIGNED_BYTE, mat.data);
};
static auto rebind_texture=[](cv::Mat& mat, ImTextureID& texture_id, bool need_to_rgba)
{
    if (mat.empty())
        return;
    if (need_to_rgba)
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2RGBA);
    glBindTexture(GL_TEXTURE_2D, texture_cast(texture_id));
    glTexImage2D(GL_TEXTURE_2D, 0, gl_texture_color_format, mat.cols, mat.rows, 0, gl_texture_color_format, GL_UNSIGNED_BYTE, mat.data);
};
    

void debugger::init(ImGuiIO& io) {
    this->font = io.Fonts->AddFontFromFileTTF("resource/zh-cn.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    ctx = create_tracker_context();
    plot_ctx = ImPlot::CreateContext();
    ctx->variables->sync_variables_frame = [this](std::string key, cv::Mat& mat){
        inspect_pool.async_update(key, mat);
    };
    inspect_pool.release_texture = [this](ImTextureID texture_id){
        if (texture_id == nullptr)
            return;
        auto gl_texture = static_cast<GLuint>(reinterpret_cast<uintptr_t>(texture_id));
        glDeleteTextures(1, &gl_texture);
        texture_id = nullptr;
    };
    inspect_pool.mat_to_texture =[](cv::Mat& mat, ImTextureID& texture_id)
    {
        if (mat.empty())
			return;
        if (texture_id == nullptr)
            return bind_texture(mat, texture_id, mat.channels()!=4);
        return rebind_texture(mat, texture_id, mat.channels()!=4);
    };
}
void debugger::next_frame(ImGuiIO& io){
    inspect_pool.bind_texture();
    inspect_pool.task_process();
    inspect_pool.render();

    ImGui::Begin("Debugger");
    ImGui::Text("耗时：%.3f ms 帧率： (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    if(ImGui::Button("RGBA"))
    {
        gl_texture_color_format = GL_RGBA;
    }
    ImGui::SameLine();
    if (ImGui::Button("BGRA"))
    {
        gl_texture_color_format = GL_BGRA_EXT;
    }
    if (ImGui::Button("采集桌面"))
    {
        ctx->variables->handle = tianli::genshin::create_genshin_handle(tianli::genshin::genshin_handle::hanlde_type::unknown);
    }
    ImGui::SameLine();
    if (ImGui::Button("采集原神"))
    {
        ctx->variables->handle = tianli::genshin::create_genshin_handle(tianli::genshin::genshin_handle::hanlde_type::official);
    }
    if (ImGui::Button("wgc截图"))
    {
        ctx->pool->pause();
        if (ctx->variables->source)
            ctx->variables->source->uninitialized();
        ctx->variables->source = tianli::frame::create_capture_source(tianli::frame::capture_source::source_type::window_graphics);
        HWND handle{};
        if (ctx->variables->handle && ctx->variables->handle->get_handle(handle))
            ctx->variables->source->set_capture_handle(handle);
        ctx->pool->resume();
    }
    ImGui::SameLine();
    if(ImGui::Button("bitblt截图"))
    {
        ctx->pool->pause();
        if (ctx->variables->source)
            ctx->variables->source->uninitialized();
        ctx->variables->source = tianli::frame::create_capture_source(tianli::frame::capture_source::source_type::bitblt);
        ctx->variables->source->set_source_handle_callback([this]()->HWND{
            HWND handle{};
            if (ctx->variables->handle && ctx->variables->handle->get_handle(handle))
                return handle;
            return nullptr;
        });
        ctx->pool->resume();
    }
    ImGui::End();
    static auto begin = std::chrono::system_clock::now();

    auto lines = ctx->variables->lines.clone();
    std::unordered_map<std::string, std::vector<ImVec2>> data;
    std::unordered_map<std::string, std::vector<ImVec2>> diff_data;
    float count = 0;
    for (auto& [key,line] : lines)
	{
        count++;
		std::vector<ImVec2> line_data;
		for (auto& point : line)
		{
			line_data.emplace_back((float)std::chrono::duration_cast<std::chrono::milliseconds>(point-begin).count(), count);
		}
		data.emplace(key,line_data);
        std::vector<ImVec2> diff_line_data;
        if(line_data.size() <= 2)
            continue;
        for (int i = 1; i < line_data.size() / 2; i++)
        {
            auto diff = line_data[i*2].x - line_data[i*2-1].x;
            auto diff_time = line_data[i*2].x;
            diff_line_data.emplace_back(diff_time, diff);
        }
        diff_data.emplace(key,diff_line_data);
	}
    ImGui::Begin("Plot tick");
    ImGui::Text("Lines: %d", lines.size());
    static int range = std::chrono::milliseconds(1000).count();
    ImGui::DragInt("Range:", &range);
    auto now = std::chrono::system_clock::now() - begin;
    auto now_time = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();

    if (ImPlot::BeginPlot("Line Plot", "x", "y", ImVec2(-1,-1))) {
        ImPlot::SetupAxisLimits(ImAxis_X1,now_time - range, now_time, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0,6);
        //ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 5.0f);
        for(auto& [key,line] : data){
            ImPlot::PlotLine(key.c_str(), &line[0].x, &line[0].y, line.size(), ImPlotLineFlags_Segments, 0, 2*sizeof(float));
        }
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
    }
    ImGui::End();

    ImGui::Begin("Diff Plot");
    if (ImPlot::BeginPlot("Diff Plot", "x", "y", ImVec2(-1,-1))) {
        ImPlot::SetupAxisLimits(ImAxis_X1,now_time - range, now_time, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0,6);
        //ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 5.0f);
        for(auto& [key,line] : diff_data){
            if (line.size() <= 2)
                continue;
            ImPlot::PlotLine(key.c_str(), &line[0].x, &line[0].y, line.size(), 0, 0, sizeof(ImVec2));
        }
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
    }
    ImGui::End();

    is_need_new_frame =true;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void debugger::destory(ImGuiIO& io){

    if(ctx->variables->source)
        ctx->variables->source->uninitialized();
    ctx->variables->source = tianli::frame::create_capture_source(tianli::frame::capture_source::source_type::bitblt);
    if (ctx->variables->source)
        ctx->variables->source->uninitialized();
    ctx->variables->source = nullptr;
    ctx = nullptr;
    ImPlot::DestroyContext(plot_ctx);
    ctx.reset();
}