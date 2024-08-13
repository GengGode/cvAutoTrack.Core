#pragma once
#include "imapp/imapp.h"
#include "imgui_implot/implot.h"
#include "imgui_implot/implot_internal.h"
#include "comments/image_inspect_pool.hpp"

class tracker_context;
namespace tianli::genshin{
    class genshin_handle;
}

class debugger : public imapp
{
public:
    debugger(window_create_params params) : imapp(params) {}
    
public:
    void init(ImGuiIO& io) override;
    void next_frame(ImGuiIO& io) override;
    void destory(ImGuiIO& io) override
    {
        ImPlot::DestroyContext(plot_ctx);
        ctx.reset();
    }
public:
    ImPlotContext* plot_ctx =nullptr;
    image_inspect_pool inspect_pool;
    std::shared_ptr<tracker_context> ctx;
    std::shared_ptr<tianli::genshin::genshin_handle> genshin;
};

#include <chrono>
#include <future>
#include <mutex>
#include <thread>
//#include <opencv2/core/mat.hpp>

#include <iostream>
#include <format>
#include <frame.include.h>

struct time_frame{
    std::chrono::microseconds frame_time;
    //cv::Mat image;
};
struct time_lines
{
    std::unordered_map<std::string_view, std::vector<std::chrono::system_clock::time_point>> lines;
    std::mutex                                                                          mutex;
    void add_point(std::string_view name, std::chrono::system_clock::time_point time)
	{
		std::lock_guard<std::mutex> lock(mutex);
		lines[name].push_back(time);
	}
    auto clone(){
		std::lock_guard<std::mutex> lock(mutex);
		return lines;
	}
    std::vector<std::vector<std::chrono::system_clock::time_point>> clone_vector() {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<std::vector<std::chrono::system_clock::time_point>> result;
        for (auto& [name, line] : lines)
        {
            result.push_back(line);
        }
        return result;
    }
};

struct variable_pool {
    std::chrono::microseconds capture_interval = std::chrono::milliseconds(33);// / 1000;
    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point capture_until = current_time + capture_interval;
    size_t capture_frame_count = 0;
    std::list<std::chrono::system_clock::time_point> capture_times;

    std::shared_ptr<tianli::frame::capture_source> source = tianli::frame::create_capture_source(tianli::frame::frame_source::source_type::window_graphics);

    time_frame current_frame;
    cv::Mat frame;

    time_lines lines;
    std::function<void(std::string, cv::Mat&)> sync_variables_frame;
};
struct task;
struct tasks {
    std::shared_ptr<task> capture;
    std::shared_ptr<task> pre_process;
    std::shared_ptr<task> partial_match;
    std::shared_ptr<task> overall_match;
    std::shared_ptr<task> data_fusion;
};
struct condition_variable_group {
    std::condition_variable capture;
    std::condition_variable pre_process;
    std::condition_variable partial_match;
    std::condition_variable overall_match;
    std::condition_variable data_fusion;
};

struct task {
    std::mutex mutex;
    variable_pool& vars;
    std::condition_variable &cv;
    condition_variable_group &cvs;
    task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : vars(vars), cv(cv), cvs(cvs) {
        start();
    }
    ~task() {
        stop();
    }

    virtual void init() {

    }
    virtual void cycle() {
    };

    virtual void start() {
        if (thread.joinable())
            return;
        thread = std::jthread(&task::run, this);
	}
    virtual void stop() {
		thread.request_stop();
        cv.notify_all();
        if (thread.joinable()) {
			thread.join();
		}
	}
    virtual void pause() {

    }
    void run() {
        stop_token = thread.get_stop_token();
        init();
        while (!stop_token.stop_requested()) {
            cycle();
        }
    }
    std::jthread thread;
    std::stop_token stop_token;
};

struct capture_task :task {
    capture_task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : task(vars, cv, cvs) {}
    void cycle() override
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait_until(lock, vars.capture_until, [this]() {
            auto now = std::chrono::system_clock::now();
            return now >= vars.capture_until;
            });
        if (stop_token.stop_requested())
            return;

        vars.lines.add_point("采集", std::chrono::system_clock::now());
        if (vars.source->get_frame(vars.frame))
        {
           // std::cout << std::format("capture frame {}\n", vars.capture_frame_count);
        }
		else
		{
			//std::cout << std::format("capture frame {} failed\n", vars.capture_frame_count);
		}
        vars.lines.add_point("采集", std::chrono::system_clock::now());

        vars.current_time = std::chrono::system_clock::now();
        vars.capture_until = vars.current_time + vars.capture_interval;

        if (vars.capture_times.size() > 2)
        {
            //std::cout << std::format("capture frame interval = {}ms\n", (vars.current_time - vars.capture_times.back()).count()/10000.0);
        }
        vars.capture_times.push_back(vars.current_time);

        vars.capture_frame_count += 1;
        //std::cout << std::format("cycle capture_frame_count = {}\n", vars.capture_frame_count);

        cvs.pre_process.notify_one();
    }
};
struct pre_process_task :task {
    pre_process_task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : task(vars, cv, cvs) {}
    void cycle() override
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);
        if (stop_token.stop_requested())
            return;
        vars.lines.add_point("预处理", std::chrono::system_clock::now());

        if(vars.sync_variables_frame)
            vars.sync_variables_frame("frame", vars.frame);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //std::cout<< std::format("from capture frame {}\n", vars.capture_frame_count);

        //auto available_frame = get_available_frame(frame);
        //sync_variable("available_frame", available_frame);
        vars.lines.add_point("预处理", std::chrono::system_clock::now());

        cvs.partial_match.notify_one();
        cvs.overall_match.notify_one();
    }
};
struct partial_match_task :task {
    partial_match_task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : task(vars, cv, cvs) {}
    void cycle() override
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);
        if (stop_token.stop_requested())
            return;
        vars.lines.add_point("局部匹配", std::chrono::system_clock::now());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

       // std::cout<< std::format("λ�� frame {}\n", vars.capture_frame_count);

        //auto displacement = calc_relative_displacement(available_frame);
        //sync_variable("displacement", displacement);
        vars.lines.add_point("局部匹配", std::chrono::system_clock::now());

        cvs.data_fusion.notify_one();
    }
};
struct overall_match_task :task {
    overall_match_task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : task(vars, cv, cvs) {}
    void cycle() override
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);
        if (stop_token.stop_requested())
            return;
        vars.lines.add_point("全局匹配", std::chrono::system_clock::now());

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
       // std::cout<< std::format("ȫ��λ�� frame {}\n", vars.capture_frame_count);

        //auto position = calc_global_position(available_frame);
        //sync_variable("position", position);
        vars.lines.add_point("全局匹配", std::chrono::system_clock::now());

        cvs.data_fusion.notify_one();
    }
};
struct data_fusion_task : task {
    data_fusion_task(variable_pool& vars, std::condition_variable& cv, condition_variable_group& cvs) : task(vars, cv, cvs) {}
    void cycle() override
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock);
        if (stop_token.stop_requested())
            return;
        vars.lines.add_point("数据融合", std::chrono::system_clock::now());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

       // std::cout<< std::format("�ں� frame {}\n", vars.capture_frame_count);
        vars.lines.add_point("数据融合", std::chrono::system_clock::now());
    }
};

struct task_pool {
    condition_variable_group cvs;
    tasks ts;
    task_pool(variable_pool& vars) {
        ts.capture = std::make_shared<capture_task>(std::ref(vars),std::reference_wrapper(cvs.capture), std::ref(cvs));
        ts.pre_process = std::make_shared<pre_process_task>(std::ref(vars),std::reference_wrapper(cvs.pre_process), std::ref(cvs));
        ts.partial_match = std::make_shared<partial_match_task>(std::ref(vars),std::reference_wrapper(cvs.partial_match), std::ref(cvs));
        ts.overall_match = std::make_shared<overall_match_task>(std::ref(vars),std::reference_wrapper(cvs.overall_match), std::ref(cvs));
        ts.data_fusion = std::make_shared<data_fusion_task>(std::ref(vars),std::reference_wrapper(cvs.data_fusion), std::ref(cvs));

        start();
    }
    ~task_pool(){
        stop();
    }

    void start(){}
    void stop(){}
    void pause(){}
};


class logger_interface     { };
class config_resources     { };
class tracker_context
{
public:
    std::shared_ptr<logger_interface> logger     = nullptr;
    std::shared_ptr<config_resources> resource   = nullptr;
    std::shared_ptr<variable_pool>    variables  = nullptr;
    std::shared_ptr<task_pool>        pool       = nullptr;
public:
    tracker_context()
	{
		logger = std::make_shared<logger_interface>();
		resource = std::make_shared<config_resources>();
		variables = std::make_shared<variable_pool>();
		pool = std::make_shared<task_pool>(std::ref(*variables));
	}
    tracker_context(std::shared_ptr<variable_pool> variables, std::shared_ptr<task_pool> pool, std::shared_ptr<logger_interface> logger, std::shared_ptr<config_resources> resource)
        : variables(variables), pool(pool), logger(logger), resource(resource) {}
};
static std::shared_ptr<tracker_context> create_tracker_context()
{
    return std::make_shared<tracker_context>();
}