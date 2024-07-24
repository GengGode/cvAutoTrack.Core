#pragma once
#include <memory>
#include <Windows.h>

namespace tianli::genshin
{
    class genshin_handle
    {
    public:
    enum class hanlde_type
    {
        unknown,
        official,
        cloud_official,
        cloud_tencent,
        cloud_netease,
        unity_debug,
    };
    public:
        hanlde_type type = hanlde_type::unknown;
    public:
        genshin_handle() = default;
        virtual ~genshin_handle() = default;
        virtual bool get_handle(HWND& handle) = 0;
        // virtual bool get_window_title(std::wstring& title) = 0;
        // virtual bool get_window_title(std::string& title) = 0;
        // virtual bool get_window_class(std::wstring& class_name) = 0;
        // virtual bool get_window_class(std::string& class_name) = 0;
    };

    std::shared_ptr<genshin_handle> create_genshin_handle(genshin_handle::hanlde_type type = genshin_handle::hanlde_type::official);
} // namespace tianli::genshin