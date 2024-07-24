#include "frame.include.h"
#include "capture/capture.bitblt.h"
#include "capture/capture.print_window.h"
#include "capture/capture.window_graphics.h"
#include "local/local.picture.h"
#include "local/local.video.h"

namespace tianli::frame
{
    std::shared_ptr<local_source> create_local_source(frame_source::source_type type)
    {
        switch (type)
        {
        case frame_source::source_type::video:
            return std::make_shared<local::local_video>();
        case frame_source::source_type::picture: 
            return std::make_shared<local::local_picture>();
        default:
            return nullptr;
        }
    }    
    std::shared_ptr<capture_source> create_capture_source(frame_source::source_type type)
    {
        switch (type)
        {
        case frame_source::source_type::bitblt:
            return std::make_shared<capture::capture_bitblt>();
        case frame_source::source_type::print_window:
            return std::make_shared<capture::capture_print_window>();
        case frame_source::source_type::window_graphics:
            return std::make_shared<capture::capture_window_graphics>();
        default:
            return nullptr;
        }
    }
} // namespace tianli::frame
