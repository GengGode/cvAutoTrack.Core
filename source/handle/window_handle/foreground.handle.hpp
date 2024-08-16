#include "../handle.include.h"
#include <array>
#include <string>

namespace tianli::handle
{
    class foreground_handle : public handle_source
    {
    public:
        foreground_handle() { type = hanlde_type::foreground; }
    public:
        std::optional<HWND> get_handle() override
        {
            auto handle = GetForegroundWindow();
            if (handle != nullptr)
                return handle;
            return std::nullopt;
        }
    };
}