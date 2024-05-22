
find_package(OpenCV REQUIRED)

add_library(cvAutoTrack.Core SHARED)
set(library_target_name cvAutoTrack.Core)

# utf8
target_compile_options(${library_target_name} PRIVATE /utf-8)
# use c++17
target_compile_features(${library_target_name} PUBLIC /std:c++17)
target_compile_definitions(${library_target_name} PUBLIC CVAUTOTRACK_CORE_EXPORTS)

target_include_directories(${library_target_name} PUBLIC ${include_dir})
target_include_directories(${library_target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${library_target_name} PRIVATE core.inface.cpp)

set_target_properties(${library_target_name} PROPERTIES FOLDER "core")

# 需要运行脚本将头文件转为cs原生代码  ClangSharpPInvokeGenerator --file cvAutoTrack.Core.h --namespace cvAutoTrack.Core.Native --output "cvAutoTrack.Core.Native.cs"
# 先拷贝头文件到输出目录
add_custom_command(TARGET ${library_target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${include_dir}/cvAutoTrack.Core.h ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET ${library_target_name} POST_BUILD
    COMMAND ClangSharpPInvokeGenerator --file cvAutoTrack.Core.h --namespace cvAutoTrack.Core.Native --output "cvAutoTrack.Core.Native.cs"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET ${library_target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${library_target_name}> ${CMAKE_SOURCE_DIR}/tests/csharp
)
