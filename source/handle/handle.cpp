#include "handle.include.h"
#include "window_handle/foreground.handle.hpp"
#include "window_handle/desktop.handle.hpp"
#include "genshin_window/genshin.official.hpp"

namespace tianli::handle
{
    std::shared_ptr<handle_source> create_handle_source(handle_source::hanlde_type type)
    {
        switch (type)
        {
        case handle_source::hanlde_type::foreground:
            return std::make_shared<foreground_handle>();
        case handle_source::hanlde_type::desktop:
            return std::make_shared<desktop_handle>();
        case handle_source::hanlde_type::genshin_official:
            return std::make_shared<genshin_official>();
        default:
            return nullptr;
        }
    }
} // namespace tianli::handle