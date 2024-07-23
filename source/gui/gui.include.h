#pragma once
#include <chrono>
#include <Windows.h>
#include <opencv2/core.hpp>

namespace tianli::gui
{
    class gui_inferface
    {
    public:
        gui_inferface() = default;
        virtual ~gui_inferface() = default;

        virtual bool initialization() { return false; }
        virtual bool uninitialized() { return false; }
        virtual bool execute() { return false; }
    };
    std::shared_ptr<gui_inferface> create_gui();
} // namespace tianli::gui
