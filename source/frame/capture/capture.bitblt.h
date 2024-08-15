#pragma once
#include "../frame.include.h"
#include "utils/utils.window_scale.h"

namespace tianli::frame::capture
{
    class capture_bitblt : public capture_source
    {
    public:
        capture_bitblt() { type = source_type::bitblt; }
        ~capture_bitblt() override = default;

    public:
        bool initialization() override
        {
            if (is_callback)
                source_handle = source_handle_callback();
            if (IsWindow(source_handle) == false)
                return false;
            screen_scale = utils::window_scale::get_screen_scale(source_handle);
            if (GetWindowRect(source_handle, &rect) == false)
                return false;
            if (GetClientRect(source_handle, &client_rect) == false)
                return false;
            client_size = { (int)std::round(screen_scale * (client_rect.right - client_rect.left)), (int)std::round(screen_scale * (client_rect.bottom - client_rect.top)) };

            screen = GetDC(source_handle);//CreateDCA("DISPLAY", NULL, NULL, NULL);
            compdc = CreateCompatibleDC(screen);
            bitmap = CreateCompatibleBitmap(screen, client_size.width, client_size.height);
            SelectObject(compdc, bitmap);

            is_initialized = true;
            return true;
        }
        bool uninitialized() override 
        {
            is_initialized = false;

            DeleteObject(bitmap);
            DeleteDC(compdc);
            ReleaseDC(source_handle, screen);
            return true; 
        }
        bool set_capture_handle(HWND handle = 0) override
        {
            if (handle == nullptr)
                return false;
            if (handle == source_handle)
                return true;
            if (uninitialized() == false)
                return false;
            source_handle = handle;
            if (initialization() == false)
                return false;
            return true;
        }
        bool set_source_handle_callback(std::function<HWND()> callback) override
        {
            if (callback == nullptr)
                return false;
            if (uninitialized() == false)
                return false;
            source_handle_callback = callback;
            is_callback = true;
            if (initialization() == false)
                return false;
            return true;
        }
        bool get_frame(cv::Mat& frame) override
        {
            if (is_initialized == false)
                return false;

            if(source_frame.empty())
                source_frame.create(client_size, CV_MAKETYPE(CV_8U, 4));
            if (source_frame.cols != client_size.width || source_frame.rows != client_size.height)
                source_frame.create(client_size, CV_MAKETYPE(CV_8U, 4));

            BitBlt(compdc, 0, 0, client_size.width, client_size.height, screen, 0, 0, SRCCOPY);
            GetBitmapBits(bitmap, client_size.width * client_size.height * 4, source_frame.data);

            if (client_rect.left!=0 || client_rect.top!=0 || client_size.width != client_size.width || client_size.height!= client_size.width)
                source_frame = source_frame(cv::Rect(client_rect.left, client_rect.top, client_size.width, client_size.height));
            if (source_frame.empty())
                return false;
            if (source_frame.cols < 480 || source_frame.rows < 360)
                return false;
            frame = source_frame;
            return true;
        }

    private:
        RECT rect = { 0, 0, 0, 0 };
        RECT client_rect = { 0, 0, 0, 0 };
        HDC screen = nullptr;
        HDC compdc = nullptr;
        HBITMAP bitmap = nullptr;
        double screen_scale = 1.0;
        cv::Size client_size;
    };

} // namespace tianli::frame::capture
