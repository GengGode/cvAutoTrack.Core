#include "frame.include.h"
#include "capture/capture.bitblt.h"
#include "capture/capture.window_graphics.h"
#include "local/local.picture.h"
#include "local/local.video.h"

#include <logger.include.h>

namespace tianli::frame
{
    frame_source::frame_source(std::shared_ptr<global::logger_interface> logger): logger(logger)
    {
        if (logger == nullptr)
        {
            this->logger = tianli::global::create_logger();//std::make_shared<global::logger_interface>();
        }
    }

    std::shared_ptr<local_source> create_local_source(frame_source::source_type type, std::shared_ptr<global::logger_interface> logger)
    {
        switch (type)
        {
        case frame_source::source_type::video:
            return std::make_shared<local::local_video>(logger);
        case frame_source::source_type::picture: 
            return std::make_shared<local::local_picture>(logger);
        default:
            return nullptr;
        }
    }    
    std::shared_ptr<capture_source> create_capture_source(frame_source::source_type type, std::shared_ptr<global::logger_interface> logger)
    {
        switch (type)
        {
        case frame_source::source_type::bitblt:
            return std::make_shared<capture::capture_bitblt>(logger);
        case frame_source::source_type::window_graphics:
            return std::make_shared<capture::capture_window_graphics>(logger);
        default:
            return nullptr;
        }
    }
} // namespace tianli::frame
