#pragma once

#include <Windows.h>

#include <opencv2/core.hpp>

#include <logger.include.h>

namespace tianli::frame
{
    class frame_source
    {
    public:
        enum class source_type
        {
            unknown,
            bitblt,
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
        std::shared_ptr<logger::logger_interface> logger;
        bool is_initialized = false;
        bool is_callback = false;
        bool has_frame_rect_callback = false;
        std::function<cv::Rect(cv::Rect)> frame_rect_callback;

    public:
        static std::shared_ptr<frame_source> create(source_type type, std::shared_ptr<logger::logger_interface> logger = nullptr);

    public:
        frame_source(std::shared_ptr<logger::logger_interface> logger = nullptr) : logger(logger)
        {
            if (logger == nullptr)
            {
                this->logger = logger::logger_interface::create(logger::logger_interface::log_type::console);
            }
        }
        virtual ~frame_source() = default;
        virtual bool initialization() { return false; }
        virtual bool uninitialized() { return false; }
        virtual bool get_frame(cv::Mat &frame) = 0;
        virtual bool set_capture_handle(HWND handle) = 0;
        virtual bool set_local_frame(cv::Mat frame) = 0;
        virtual bool set_local_file(std::string file) = 0;
        virtual bool set_source_handle_callback(std::function<HWND()> callback) = 0;
        virtual bool set_source_frame_callback(std::function<cv::Mat()> callback) = 0;
        virtual bool set_frame_rect_callback(std::function<cv::Rect(cv::Rect)> callback) = 0;
    };

} // namespace tianli::frame
