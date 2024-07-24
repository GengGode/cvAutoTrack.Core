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
                handle = FindWindowW(L"UnityClassWnd", name.c_str());
                if(handle != nullptr)
                    return true;
            }
            return false;
        }
    private:
        const static inline std::array<std::wstring, 3> genshin_names={
            L"原神", L"원신", L"Genshin Impact"
        };
    };
}