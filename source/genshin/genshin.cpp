#include "global.genshin.h"
#include "genshin_window/genshin.official.hpp"

namespace tianli::genshin
{
    std::shared_ptr<genshin_handle> create_genshin_handle(genshin_handle::hanlde_type type)
    {
        return std::make_shared<genshin_official>();
    }
} // namespace tianli::genshin