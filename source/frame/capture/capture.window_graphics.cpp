#include "capture.window_graphics.h"

namespace tianli::frame::capture
{
    bool capture_window_graphics::get_frame(cv::Mat& frame)
    {
        if(is_initialized == false)
            return false;

        if (frame_pool == nullptr)
        {
            uninitialized();
            if (initialization() == false)
                return false;
        }

        auto& current_texture = texture_using == true ? texture_swap : texture;
        if (current_texture == nullptr)
            return false;

        texture_using = true;
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        if (SUCCEEDED(context->Map(current_texture, 0, D3D11_MAP_READ, 0, &mapped_resource)))
        {
            D3D11_TEXTURE2D_DESC desc;
            current_texture->GetDesc(&desc);
            cv::Mat mat(desc.Height, desc.Width, CV_8UC4, mapped_resource.pData, mapped_resource.RowPitch);
            //mat.copyTo(frame);
            source_frame = mat;
            context->Unmap(current_texture, 0);
        }
        texture_using = false;

        if (source_frame.empty())
            return false;
        if (source_frame.cols < 480 || source_frame.rows < 360)
            return false;
        frame = source_frame/*.clone()*/;
        return true;
}
}