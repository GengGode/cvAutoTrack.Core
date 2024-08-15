#include "global.genshin.h"
#include "genshin_window/debug.window.hpp"
#include "genshin_window/genshin.official.hpp"

namespace tianli::genshin
{
    std::shared_ptr<genshin_handle> create_genshin_handle(genshin_handle::hanlde_type type)
    {
        switch (type)
        {
        case genshin_handle::hanlde_type::unknown:
            return std::make_shared<debug_window>();
        case genshin_handle::hanlde_type::official:
            return std::make_shared<genshin_official>();
        }
        
        return std::make_shared<genshin_official>();
    }
} // namespace tianli::genshin