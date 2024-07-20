cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core-ctest)
set(library_target_name core-ctest)

add_executable(${library_target_name} main.c)

set_target_properties(${library_target_name} PROPERTIES FOLDER "tests/c")

target_link_libraries(${library_target_name} cvAutoTrack.Core)
