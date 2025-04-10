file(
    GLOB_RECURSE plugin_src_list
    *.cpp
)

add_library(echo SHARED ${plugin_src_list})
target_link_libraries(echo src utils)

# 设置动态库的输出路径，只影响这个目标
set_target_properties(echo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  # Linux/Unix
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  # Linux/Unix, for .a files if applicable
)
