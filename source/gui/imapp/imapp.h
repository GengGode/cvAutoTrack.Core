#pragma once
#include <memory>
#include <atomic>
#include <mutex>
#include <string>
#include <filesystem>
#include <execution>
#include <random>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/GL.h>
#include <tchar.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct window_opengl {
	// Data stored per platform window
	struct window_data {
		HDC hDC; // Window device context
	};

	// Data
	HGLRC            hRC;
	window_data      MainWindow;
	int              Width;
	int              Height;

    HWND                               handle;  
    WNDCLASSEXW                        wc;             
    HWND                               target_parent;  // depend window_create[parent]
    DWORD                              dwStyle;        // depend window_create[parent]
    std::shared_ptr<wchar_t[]>         app_name;       // depend window_create[parent]
    std::shared_ptr<wchar_t[]>         app_class_name; // depend window_create[parent]
    SIZE                               size;           // depend window_create[width, height]
    POINT                              position;       // depend window_create[parent, x, y]

	// Helper functions
	bool CreateDeviceWGL(HWND hWnd);

	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND create_window();
    void shutdown_window();
};

//#include <opencv2/xfeatures2d/nonfree.hpp>


//cv::Point(232, 216), cv::Point(-1, 0)
#define texture_cast(texture_id) \
	static_cast<GLuint>(reinterpret_cast<uintptr_t>(texture_id))


struct window_create_params
{
    HWND parent; // target parent window
    int width;
    int height;
    bool is_force_main_window = true;
};

#include "../gui.include.h"

class imapp : public tianli::gui::gui_inferface
{
public:
    imapp(window_create_params window_create_params);
public:
    window_opengl wgl{};

	/*****************/
	// ImGui data
	/*****************/
	ImGuiContext*              context{};
    ImFont*                    font{};

    /*****************/
    // Global Cache
    /*****************/
    ImVec4               background_color = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);

    /*****************/
    // Main Status
    /*****************/
    bool                 is_done = true;
    bool                 is_need_new_frame = true;
    window_create_params window_create;

    /*****************/
    // Runtime Value
    /*****************/
public:
    virtual void init(ImGuiIO& io){}
    virtual void next_frame(ImGuiIO& io){}
    virtual void destory(ImGuiIO& io){}

    void frame_loop();
    bool initialization() override;
    bool uninitialized() override;
    bool execute() override;
};
