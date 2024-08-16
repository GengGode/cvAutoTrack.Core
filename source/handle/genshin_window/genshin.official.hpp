#include "../handle.include.h"
#include <array>
#include <string>

namespace tianli::handle
{
    class genshin_official : public handle_source
    {
    public:
        genshin_official() { type = hanlde_type::genshin_official; }
    public:
        std::optional<HWND> get_handle() override
        {
            for(const auto& name : genshin_names)
            {
                auto handle = FindWindowW(L"UnityWndClass", name.c_str());
                if(handle != nullptr)
                    return handle;
            }
            for(const auto& name : genshin_names_utf8)
            {
                auto handle = FindWindowA("UnityWndClass", name.c_str());
                if(handle != nullptr)
                    return handle;
            }
            return std::nullopt;
        }
    private:
        const static inline std::array<std::wstring, 3> genshin_names={
            L"原神", L"원신", L"Genshin Impact"
        };
        const static inline std::array<std::string, 3> genshin_names_utf8={
            "原神", "원신", "Genshin Impact"
        };
    };
}