#include "../handle.include.h"
#include <array>
#include <string>

namespace tianli::handle
{
    class desktop_handle : public handle_source
    {
    public:
        desktop_handle() { type = hanlde_type::desktop; }
    public:
        std::optional<HWND> get_handle() override
        {
            auto handle = GetDesktopWindow();
            if (handle != nullptr)
                return handle;
            return std::nullopt;
        }
    };
}