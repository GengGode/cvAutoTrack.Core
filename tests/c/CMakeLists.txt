cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core-ctest)

add_executable(cvAutoTrack.Core-ctest main.c)

target_link_libraries(cvAutoTrack.Core-ctest cvAutoTrack.Core)
