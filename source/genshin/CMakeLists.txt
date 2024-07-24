
add_library(tianli_genshin STATIC)
add_library(tianli::genshin ALIAS tianli_genshin)
set(library_target_name tianli_genshin)

target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)

target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${library_target_name}
    PUBLIC
        global.genshin.h
    PRIVATE
        genshin.cpp
        )
set_target_properties(${library_target_name} PROPERTIES FOLDER "core/modules")