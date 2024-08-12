#include "../global.genshin.h"
#include <array>
#include <string>

namespace tianli::genshin
{
    class genshin_official : public genshin_handle
    {
    public:
        genshin_official() { type = hanlde_type::official; }
    public:
        bool get_handle(HWND& handle) override
        {
            for(const auto& name : genshin_names)
            {
                handle = FindWindowW(L"UnityWndClass", name.c_str());
                if(handle != nullptr)
                    return true;
            }
            for(const auto& name : genshin_names_utf8)
            {
                handle = FindWindowA("UnityWndClass", name.c_str());
                if(handle != nullptr)
                    return true;
            }
            return false;
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