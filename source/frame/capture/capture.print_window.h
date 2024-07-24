#pragma once
#include "../frame.include.h"
#include "utils/utils.window_scale.h"

namespace tianli::frame::capture
{
    class capture_print_window : public capture_source
    {
    public:
        capture_print_window() { type = source_type::print_window; }
        ~capture_print_window() override = default;

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


            //位图信息
            BITMAPINFO bitmap_info;
            bitmap_info.bmiHeader.biSize = sizeof(bitmap_info);
            bitmap_info.bmiHeader.biWidth = client_size.width;
            bitmap_info.bmiHeader.biHeight = client_size.height;
            bitmap_info.bmiHeader.biPlanes = 1;
            bitmap_info.bmiHeader.biBitCount = 32;
            bitmap_info.bmiHeader.biSizeImage = client_size.area();
            bitmap_info.bmiHeader.biCompression = BI_RGB;
        
            screen = ::GetWindowDC(source_handle);
            compdc = ::CreateCompatibleDC(screen);
            bitmap = ::CreateDIBSection(screen, &bitmap_info, DIB_RGB_COLORS, &bits_ptr, nullptr, 0);
            if (bitmap == nullptr)
				return false;
            bitmap_old = static_cast<HBITMAP>(SelectObject(compdc, bitmap));

            return true;
        }
        bool uninitialized() override 
        {
            DeleteObject(bitmap_old);
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
            if (source_frame.cols != client_size.width || source_frame.rows != client_size.height)
                source_frame.create(client_size, CV_MAKETYPE(CV_8U, 4));

            PrintWindow(source_handle, compdc, PW_CLIENTONLY | PW_RENDERFULLCONTENT);
            GetBitmapBits(bitmap_old, client_size.width * client_size.height * 4, source_frame.data);

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
        HBITMAP bitmap_old = nullptr;
        void* bits_ptr = nullptr;
        double screen_scale = 1.0;
        cv::Size client_size;
    };

} // namespace tianli::frame::capture
