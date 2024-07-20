#pragma once

#include <Windows.h>

#include <opencv2/core.hpp>

#include <logger.include.h>

namespace tianli::frame
{
    class logger_interface;
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
        std::shared_ptr<global::logger_interface> logger;
        bool is_initialized = false;
        bool is_callback = false;
    public:
        frame_source(std::shared_ptr<global::logger_interface> logger = nullptr);
        virtual ~frame_source() = default;
        virtual bool initialization() { return false; }
        virtual bool uninitialized() { return false; }
        virtual bool get_frame(cv::Mat& frame) = 0;
        virtual std::shared_ptr<time_frame> get_time_frame() { return nullptr; }
    };

    class local_source : public frame_source
    {
    public:
        local_source(std::shared_ptr<global::logger_interface> logger = nullptr) : frame_source(logger) { this->mode = source_mode::frame; }
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
        capture_source(std::shared_ptr<global::logger_interface> logger = nullptr) : frame_source(logger) { this->mode = source_mode::handle; }
        ~capture_source() override = default;

        virtual bool set_capture_handle(HWND handle) = 0;
        virtual bool set_source_handle_callback(std::function<HWND()> callback) = 0;
    protected:
        std::function<HWND()> source_handle_callback;
        HWND source_handle = nullptr;
        cv::Mat source_frame;
    };

    std::shared_ptr<local_source> create_local_source(frame_source::source_type type, std::shared_ptr<global::logger_interface> logger = nullptr);
    std::shared_ptr<capture_source> create_capture_source(frame_source::source_type type, std::shared_ptr<global::logger_interface> logger = nullptr);

} // namespace tianli::frame
