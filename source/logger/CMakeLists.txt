
find_package(OpenCV REQUIRED)

add_library(tianli_logger STATIC)
add_library(tianli::logger ALIAS tianli_logger)
set(library_target_name tianli_logger)

# utf8
target_compile_options(${library_target_name} PRIVATE /utf-8)
# use c++17
target_compile_features(${library_target_name} PUBLIC /std:c++17)

target_include_directories(tianli_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(tianli_logger PRIVATE logger.cpp)

set_target_properties(tianli_logger PROPERTIES FOLDER "core/modules")
