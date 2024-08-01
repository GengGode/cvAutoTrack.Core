add_library(tianli_global STATIC)
add_library(tianli::global ALIAS tianli_global)
set(library_target_name tianli_global)

project_set_options(${library_target_name})
set_target_properties(${library_target_name} PROPERTIES FOLDER "core/modules")
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${WITH_STATIC_RUNTIME}>>:DLL>")
target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${library_target_name}
    PUBLIC
        global.resource.hpp
    PRIVATE
        )

target_link_libraries(${library_target_name} PUBLIC tianli::logger)
target_link_libraries(${library_target_name}
    PUBLIC
        ${OpenCV_LIBS})