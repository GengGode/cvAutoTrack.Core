cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core-cpptest)
set(library_target_name core-cpptest)

add_executable(${library_target_name} main.cpp)

target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)
set(WITH_DYNAMIC_OPENCV OFF)
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${WITH_DYNAMIC_OPENCV}>:DLL>")
set_target_properties(${library_target_name} PROPERTIES FOLDER "tests/cpp")

target_link_libraries(${library_target_name} cvAutoTrack.Core)
target_link_libraries(${library_target_name} tianli::frame)
