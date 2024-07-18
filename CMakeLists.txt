cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core)

# set environment variable
#----------------------------------------------------
set(third_party_dir ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(depends_party_dir ${CMAKE_CURRENT_SOURCE_DIR}/depends_party)
set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/source)

# set output directory
#----------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_subdirectory(source)

add_subdirectory(tests/c)
