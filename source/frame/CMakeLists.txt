
find_package(OpenCV REQUIRED)


add_library(tianli_frame STATIC)
add_library(tianli::frame ALIAS tianli_frame)
set(library_target_name tianli_frame)

# utf8
target_compile_options(${library_target_name} PRIVATE /utf-8)
# use c++17
target_compile_features(${library_target_name} PUBLIC /std:c++17)

target_sources(${library_target_name}
    PUBLIC
        frame.include.h
    PRIVATE
        frame.cpp
        capture/capture.include.h
        capture/capture.bitblt.h
        capture/capture.window_graphics.h
        capture/capture.window_graphics.cpp

        local/local.include.h
        local/local.picture.h
        local/local.video.h
        )

set(WITH_DYNAMIC_OPENCV OFF)
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${WITH_DYNAMIC_OPENCV}>:DLL>")
set_target_properties(${library_target_name} PROPERTIES FOLDER "core/modules")

target_link_libraries(${library_target_name} PUBLIC tianli::logger)
target_link_libraries(${library_target_name}
    PUBLIC
        ${OpenCV_LIBS}
        d3d11.lib 
        d3dcompiler.lib 
        dxgi.lib)