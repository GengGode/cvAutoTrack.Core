#pragma once
#include <memory>
#include <optional>
#include <Windows.h>

namespace tianli::handle
{
    class handle_source
    {
    public:
    enum class hanlde_type
    {
        unknown,
        foreground,
        desktop,
        genshin_official,
        genshin_cloud_official,
        genshin_cloud_tencent,
        genshin_cloud_netease,
        genshin_unity_debug,
    };
    public:
        hanlde_type type = hanlde_type::unknown;
    public:
        handle_source() = default;
        virtual ~handle_source() = default;
        virtual std::optional<HWND> get_handle() = 0;
    };

    std::shared_ptr<handle_source> create_handle_source(handle_source::hanlde_type type);
} // namespace tianli::handle