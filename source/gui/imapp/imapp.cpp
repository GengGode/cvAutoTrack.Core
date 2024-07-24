#include "imapp.h"
#include <global.convect.h>

// Helper functions
bool window_opengl::CreateDeviceWGL(HWND hWnd)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    MainWindow.hDC = ::GetDC(hWnd);
    if (!this->hRC)
        this->hRC = wglCreateContext(MainWindow.hDC);
    return true;
}

LRESULT WINAPI window_opengl::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            // Width = LOWORD(lParam);
            // Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

HWND window_opengl::create_window()
{
    this->wc = { sizeof(WNDCLASSEXW), CS_OWNDC, window_opengl::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, this->app_class_name.get(), nullptr };
    auto register_res = ::RegisterClassExW(&this->wc);
    if (register_res == 0)
        return nullptr;
    this->handle = ::CreateWindowW(this->wc.lpszClassName, this->app_name.get(), this->dwStyle, this->position.x, this->position.y, this->size.cx, this->size.cy, this->target_parent, nullptr, this->wc.hInstance, nullptr);

    // Initialize OpenGL
    if (!this->CreateDeviceWGL(this->handle))
    {
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(this->handle, this->MainWindow.hDC);
        ::DestroyWindow(this->handle);
        ::UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance);
        return nullptr;
    }
    wglMakeCurrent(this->MainWindow.hDC, this->hRC);

    // Show the window
    ::ShowWindow(this->handle, SW_SHOWDEFAULT);
    ::UpdateWindow(this->handle);
    return this->handle;
}

void window_opengl::shutdown_window()
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(this->handle, this->MainWindow.hDC);
    wglDeleteContext(this->hRC);
    ::DestroyWindow(this->handle);
    ::UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance);
}


imapp::imapp(window_create_params window_create) : window_create(window_create)
{ 
        wgl.dwStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_CLIPSIBLINGS;
        if (window_create.parent == nullptr)
            wgl.dwStyle = WS_OVERLAPPEDWINDOW;
        if (window_create.is_force_main_window)
            wgl.dwStyle = WS_OVERLAPPEDWINDOW;
        if (window_create.is_force_main_window)
            window_create.parent = nullptr;
        wgl.target_parent = window_create.parent;

        wgl.app_name = tianli::global::to_shared_wstring(L"imapp");// + std::to_wstring(reinterpret_cast<int64_t>(window_create.parent)));
        wgl.app_class_name = tianli::global::to_shared_wstring(L"ImAppClass");// + std::to_wstring(reinterpret_cast<int64_t>(window_create.parent)));
        
        wgl.size = { window_create.width, window_create.height };

        wgl.position = { 0, 0 };
}

void imapp::frame_loop()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    while (!this->is_done)
    {
        {
            MSG msg{};
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                {
                    this->is_done = true;
                }
            }
            if (msg.message == WM_NULL && this->is_need_new_frame == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            if (this->is_done)
                break;
            this->is_need_new_frame = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        this->next_frame(io);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, this->wgl.Width, this->wgl.Height);
        glClearColor(this->background_color.x, this->background_color.y, this->background_color.z, this->background_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ::SwapBuffers(this->wgl.MainWindow.hDC);
    }
}

bool imapp::initialization()
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    this->wgl.create_window();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    this->context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.IniFilename = nullptr; 
    
    //this->font = io.Fonts->AddFontFromFileTTF("resource/zh-cn.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(this->wgl.handle);
    ImGui_ImplOpenGL3_Init();
    this->init(io);

    // Main loop
    this->is_done = false;
    return true;
}

bool imapp::uninitialized()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    this->wgl.shutdown_window();
    return true;
}

bool imapp::execute()
{
    if (!this->initialization())
        return false;
    this->frame_loop();
    return this->uninitialized();
}

// Main code
int gui_main(imapp& app)
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    app.wgl.create_window();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    app.context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.IniFilename = nullptr; 
    
    app.font = io.Fonts->AddFontFromFileTTF("resource/zh-cn.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(app.wgl.handle);
    ImGui_ImplOpenGL3_Init();
    app.init(io);

    // Main loop
    app.is_done = false;
    while (!app.is_done)
    {
        {
            MSG msg{};
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                {
                    app.is_done = true;
                }
            }
            if (msg.message == WM_NULL && app.is_need_new_frame == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            if (app.is_done)
                break;
            app.is_need_new_frame = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        app.next_frame(io);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, app.wgl.Width, app.wgl.Height);
        glClearColor(app.background_color.x, app.background_color.y, app.background_color.z, app.background_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ::SwapBuffers(app.wgl.MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    app.wgl.shutdown_window();

    return 0;
}

int gui_main(std::shared_ptr<imapp> app)
{
    imapp& app_ref = *app.get();
    return gui_main(app_ref);
}
