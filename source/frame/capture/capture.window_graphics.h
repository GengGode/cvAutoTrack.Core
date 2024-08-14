#pragma once
#include "../frame.include.h"
#include "utils/utils.window_graphics.h"
#include "utils/utils.window_scale.h"

namespace tianli::frame::capture
{
    class capture_window_graphics : public capture_source
    {
        const bool                                                            is_async = false;

        winrt::Windows::Graphics::Capture::GraphicsCaptureItem                                  item{ nullptr };
        winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice                          device{ nullptr };
        winrt::com_ptr<ID3D11DeviceContext>                                                     context;
        winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool                           frame_pool{ nullptr };
        winrt::Windows::Graphics::Capture::GraphicsCaptureSession                               session{ nullptr };
        winrt::Windows::Graphics::SizeInt32                                                     last_size{};
        winrt::Windows::Graphics::Capture::GraphicsCaptureItem::Closed_revoker                  closed;
        winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::FrameArrived_revoker     frame_arrived;
        ID3D11Texture2D*                                                                        texture{ nullptr };
        ID3D11Texture2D*                                                                        texture_swap{ nullptr };
        std::atomic<bool>                                                                       texture_using{ };
        std::atomic<bool>                                                                       texture_writting{ };
        DXGI_FORMAT                                                                             format{};
        uint32_t                                                                                texture_width{};
        uint32_t                                                                                texture_height{};
        D3D11_BOX                                                                               client_box{};

        void on_closed(winrt::Windows::Graphics::Capture::GraphicsCaptureItem const&,
            winrt::Windows::Foundation::IInspectable const&)
        {
            is_initialized = false;
        }
        void on_frame_arrived(winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const&,
            winrt::Windows::Foundation::IInspectable const&)
        {
            auto frame = frame_pool.TryGetNextFrame();
            if (frame == nullptr)
                return;
            auto frame_size = frame.ContentSize();
            if (frame_size != last_size)
            {
                frame_pool.Recreate(device, winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized, 2, frame_size);
                last_size = frame_size;
            }
            auto frame_surface = utils::window_graphics::GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());

            D3D11_TEXTURE2D_DESC desc;
            frame_surface->GetDesc(&desc);
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            desc.MiscFlags = 0;

            auto& current_texture = texture_using == true ? texture_swap : texture;

            texture_using = true;
            if(current_texture)
            {    
                if (desc.Width != texture_width || desc.Height != texture_height || desc.Format != format)
                {
                    current_texture->Release();
                    current_texture = nullptr;
                }
            }
            if (current_texture == nullptr)
                utils::window_graphics::graphics_global::get_instance().d3d_device->CreateTexture2D(&desc, nullptr, &current_texture);
            bool client_box_available = utils::window_graphics::get_client_box(source_handle, desc.Width, desc.Height, &client_box);
            if (client_box_available)
                context->CopySubresourceRegion(current_texture, 0, 0, 0, 0, frame_surface.get(), 0, &client_box);
            else
                context->CopyResource(current_texture, frame_surface.get());
            texture_using = false;
        }
    public:
        capture_window_graphics()
        {
            this->type = source_type::window_graphics;
            auto init_global = utils::window_graphics::graphics_global::get_instance();
        }
        ~capture_window_graphics() override { uninitialized(); }

    public:
        bool initialization() override
        {
            if (this->is_initialized)
                return true;

            if (this->is_callback)
                this->source_handle = this->source_handle_callback();
            if (IsWindow(this->source_handle) == false)
                return false;

            item = utils::window_graphics::CreateCaptureItemForWindow(this->source_handle);

            if (item == nullptr)
                return false;

            last_size = item.Size();

            device = utils::window_graphics::CreateDirect3DDevice(utils::window_graphics::graphics_global::get_instance().dxgi_device.get());
            utils::window_graphics::graphics_global::get_instance().d3d_device->GetImmediateContext(context.put());

            if(is_async == false)
            {            
                frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(device, static_cast<winrt::Windows::Graphics::DirectX::DirectXPixelFormat>(87), 2, last_size);
                session = frame_pool.CreateCaptureSession(item);
                utils::window_graphics::set_capture_session_property(session);
                session.StartCapture();
            }
            else
            {
                frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::CreateFreeThreaded(device, static_cast<winrt::Windows::Graphics::DirectX::DirectXPixelFormat>(87), 2, last_size);
                std::thread async_create = std::thread([this]() {
                    session = frame_pool.CreateCaptureSession(item);
                    utils::window_graphics::set_capture_session_property(session);
                    session.StartCapture();
	            });
                async_create.detach();
            }

            closed = item.Closed(winrt::auto_revoke, { this, &capture_window_graphics::on_closed });
            frame_arrived = frame_pool.FrameArrived(winrt::auto_revoke, { this,&capture_window_graphics::on_frame_arrived });

            this->is_initialized = true;
            return true;
        }

        bool uninitialized() override
        {
            if (this->is_initialized == false)
                return true;
            this->is_initialized = false;

            frame_arrived.revoke();
            closed.revoke();

            if (frame_pool != nullptr)
                frame_pool.Close();
            if (session != nullptr)
                session.Close();
            session = nullptr;
            frame_pool = nullptr;
            context = nullptr;
            device = nullptr;
            item = nullptr;
            return true;
        }

        bool set_capture_handle(HWND handle = 0) override
        {
            if (handle == nullptr)
                return false;
            if (handle == this->source_handle)
                return true;
            if (uninitialized() == false)
                return false;
            this->source_handle = handle;
            if (initialization() == false)
                return false;
            this->is_callback = false;
            return true;
        }

        bool set_source_handle_callback(std::function<HWND()> callback) override
        {
            if (callback == nullptr)
                return false;
            if (uninitialized() == false)
                return false;
            this->source_handle_callback = callback;
            if (initialization() == false)
                return false;
            this->is_callback = true;
            return true;
        }

        bool get_frame(cv::Mat& frame) override;
        
    private:
        RECT source_rect = { 0, 0, 0, 0 };
        RECT source_client_rect = { 0, 0, 0, 0 };
        cv::Size source_client_size;
    };

} // namespace tianli::frame::capture
