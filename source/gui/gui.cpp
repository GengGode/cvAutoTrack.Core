#include "gui.include.h"
#include "imapp/imapp.h"
#include "debugger.hpp"

namespace tianli::gui
{
    std::shared_ptr<gui_inferface> create_gui()
    {
        return std::make_shared<debugger>(window_create_params{{},1280,720,false});
    }
} // namespace tianli::gui
