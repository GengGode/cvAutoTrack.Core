#include "../global.genshin.h"
#include <array>
#include <string>

namespace tianli::genshin
{
    class debug_window : public genshin_handle
    {
    public:
        debug_window() { type = hanlde_type::unknown; }
    public:
        bool get_handle(HWND& handle) override
        {
            handle = GetForegroundWindow();
            if (handle != nullptr)
                return true;
            return false;
        }
    };
}