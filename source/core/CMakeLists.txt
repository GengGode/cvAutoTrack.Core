add_library(cvAutoTrack.Core SHARED)
set(library_target_name cvAutoTrack.Core)

set_target_properties(${library_target_name} PROPERTIES FOLDER "core")
set_target_properties(${library_target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${WITH_STATIC_RUNTIME}>>:DLL>")
target_compile_options(${library_target_name} PRIVATE /utf-8)
target_compile_options(${library_target_name} PRIVATE /std:c++20)
target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${library_target_name} PUBLIC ${include_dir})
target_include_directories(${library_target_name} PRIVATE ${source_dir})

target_compile_definitions(${library_target_name} PRIVATE CVAUTOTRACK_CORE_EXPORTS)

target_sources(${library_target_name} PRIVATE core.inface.cpp)

# 需要运行脚本将头文件转为cs原生代码  ClangSharpPInvokeGenerator --file cvAutoTrack.Core.h --namespace cvAutoTrack.Core.Native --output "cvAutoTrack.Core.Native.cs"
# 先拷贝头文件到输出目录
# add_custom_command(TARGET ${library_target_name} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy ${include_dir}/cvAutoTrack.Core.h ${CMAKE_BINARY_DIR}
# )
# add_custom_command(TARGET ${library_target_name} POST_BUILD
#     COMMAND ClangSharpPInvokeGenerator --file cvAutoTrack.Core.h --namespace cvAutoTrack.Core.Native --output "cvAutoTrack.Core.Native.cs"
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )
# add_custom_command(TARGET ${library_target_name} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${library_target_name}> ${CMAKE_SOURCE_DIR}/tests/csharp
# )
