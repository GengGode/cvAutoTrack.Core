find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)

macro (project_set_options library_target_name)
    target_compile_options(${library_target_name} PRIVATE /utf-8)
    target_compile_options(${library_target_name} PRIVATE /std:c++20)
    target_compile_options(${library_target_name} PRIVATE /MP)
endmacro()

add_subdirectory(genshin)
# add_subdirectory(global)
add_subdirectory(logger)
add_subdirectory(frame)
add_subdirectory(core)
add_subdirectory(gui)


#add_library(cvAutoTrack.Core SHARED)
#add_library(cvAutoTrack::Core ALIAS cvAutoTrack.Core)

# Add source files