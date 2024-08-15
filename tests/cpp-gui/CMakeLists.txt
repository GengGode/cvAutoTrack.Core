cmake_minimum_required(VERSION 3.15)

project(cvAutoTrack.Core-gui-cpptest)
set(library_target_name gui-cpptest)

add_executable(${library_target_name} main.cpp)

target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)
target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)
target_compile_options(${library_target_name} PRIVATE /MP)
target_compile_options(${library_target_name} PRIVATE /Zi)
target_link_options(${library_target_name} PRIVATE /DEBUG)
target_link_options(${library_target_name} PRIVATE /OPT:REF)
target_link_options(${library_target_name} PRIVATE /OPT:ICF)
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${WITH_STATIC_RUNTIME}>>:DLL>")
set_target_properties(${library_target_name} PROPERTIES FOLDER "tests/cpp")

target_link_libraries(${library_target_name} PUBLIC tianli::gui)
target_link_libraries(${library_target_name} PUBLIC cvAutoTrack.Core)

# add_custom_command(
#     TARGET ${library_target_name}
#     PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory ARGS $<TARGET_FILE_DIR:${library_target_name}>/resource
# )
# add_custom_command(
#     TARGET ${library_target_name}
#     PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different ARGS ${source_dir}/gui/imapp/resource/*.ttf $<TARGET_FILE_DIR:${library_target_name}>/resource
# )