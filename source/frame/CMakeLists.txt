add_library(tianli_frame STATIC)
add_library(tianli::frame ALIAS tianli_frame)
set(library_target_name tianli_frame)

project_set_options(${library_target_name})
set_target_properties(${library_target_name} PROPERTIES FOLDER "core/modules")
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${WITH_STATIC_RUNTIME}>>:DLL>")
target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${library_target_name}
    PUBLIC
        frame.include.h
    PRIVATE
        frame.cpp
        capture/capture.bitblt.h
        capture/capture.window_graphics.h
        capture/capture.window_graphics.cpp

        local/local.picture.h
        local/local.video.h
        )

target_link_libraries(${library_target_name}
    PUBLIC
        ${OpenCV_LIBS}
        d3d11.lib 
        d3dcompiler.lib 
        dxgi.lib)