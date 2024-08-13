cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

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

# set global setting
#----------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
Set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake_targets")

# 如果VCPKG_TARGET_TRIPLET中包含static，则设置WITH_STATIC_RUNTIME为ON
if (VCPKG_TARGET_TRIPLET MATCHES "static")
    set(WITH_STATIC_RUNTIME ON)
else()
    set(WITH_STATIC_RUNTIME OFF)
endif()
message(STATUS "WITH_STATIC_RUNTIME: ${WITH_STATIC_RUNTIME}")

add_subdirectory(source)

add_subdirectory(tests/c)
add_subdirectory(tests/cpp)
add_subdirectory(tests/cpp-gui)
