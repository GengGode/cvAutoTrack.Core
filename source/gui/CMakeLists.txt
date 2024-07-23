
find_package(OpenCV REQUIRED)


add_library(tianli_gui STATIC)
add_library(tianli::gui ALIAS tianli_gui)
set(library_target_name tianli_gui)

target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)

target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${library_target_name} PRIVATE ${source_dir})
target_include_directories(${library_target_name} PRIVATE imapp)
target_include_directories(${library_target_name} PRIVATE imgui)
target_include_directories(${library_target_name} PRIVATE imgui/backends)
target_include_directories(${library_target_name} PRIVATE imgui_third)

# imgui source files on imgui dir
set(imgui_dir imgui)
set(imgui_backends_dir imgui/backends)
set(imgui_third_dir imgui_third)
file(GLOB imgui_sources ${imgui_dir}/*.cpp ${imgui_dir}/*.h)
set(imgui_sources ${imgui_sources} ${imgui_backends_dir}/imgui_impl_win32.cpp)
#set(imgui_sources ${imgui_sources} ${imgui_backends_dir}/imgui_impl_dx11.cpp)
set(imgui_sources ${imgui_sources} ${imgui_backends_dir}/imgui_impl_opengl3.cpp)

find_package(OpenGL REQUIRED)

target_sources(${library_target_name}
    PUBLIC
        gui.include.h
    PRIVATE
        gui.cpp
        debugger.hpp
        debugger.cpp
        imapp/imapp.h
        imapp/imapp.cpp
        
        ${imgui_sources}
        ${imgui_third_dir}/imgui_implot/implot.cpp
        ${imgui_third_dir}/imgui_implot/implot_items.cpp
        )

set(WITH_DYNAMIC_OPENCV OFF)
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${WITH_DYNAMIC_OPENCV}>:DLL>")
set_target_properties(${library_target_name} PROPERTIES FOLDER "core/modules")

target_link_libraries(${library_target_name} PUBLIC tianli::frame)
target_link_libraries(${library_target_name}
    PUBLIC
        ${OpenCV_LIBS}
        d3d11.lib 
        d3dcompiler.lib 
        dxgi.lib

        opengl32.lib
        )
        