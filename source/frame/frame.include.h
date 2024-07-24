#pragma once
#include <chrono>
#include <Windows.h>
#include <opencv2/core.hpp>

namespace tianli::frame
{
     struct time_frame {
        std::chrono::system_clock::time_point time_point;
        cv::Mat image;
    };

    class frame_source
    {
    public:
        enum class source_type
        {
            unknown,
            bitblt,
            print_window,
            window_graphics,
            video,
            picture
        };
        enum class source_mode
        {
            handle,
            frame
        };
    public:
        source_type type = source_type::unknown;
        source_mode mode = source_mode::handle;
        bool is_initialized = false;
        bool is_callback = false;
    public:
        frame_source() = default;
        virtual ~frame_source() = default;
        virtual bool initialization() { return false; }
        virtual bool uninitialized() { return false; }
        virtual bool get_frame(cv::Mat& frame) = 0;
        virtual std::shared_ptr<time_frame> get_time_frame() { return nullptr; }
    };

    class local_source : public frame_source
    {
    public:
        local_source() { this->mode = source_mode::frame; }
        ~local_source() override = default;

        virtual bool set_local_frame(cv::Mat frame) = 0;
        virtual bool set_local_file(std::string file) = 0;
        virtual bool set_source_frame_callback(std::function<cv::Mat()> callback) = 0;
    protected:
        std::function<cv::Mat()> source_frame_callback;
        std::string source_local;
        cv::Mat source_frame;
    };   

    class capture_source : public frame_source
    {
    public:
        capture_source() { this->mode = source_mode::handle; }
        ~capture_source() override = default;

        virtual bool set_capture_handle(HWND handle) = 0;
        virtual bool set_source_handle_callback(std::function<HWND()> callback) = 0;
    protected:
        std::function<HWND()> source_handle_callback;
        HWND source_handle = nullptr;
        cv::Mat source_frame;
    };

    std::shared_ptr<local_source> create_local_source(frame_source::source_type type);
    std::shared_ptr<capture_source> create_capture_source(frame_source::source_type type);

} // namespace tianli::frame
