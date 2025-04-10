# 获取所有子目录
file(GLOB dirent_list "${CMAKE_CURRENT_SOURCE_DIR}/*")
# 循环遍历子目录并添加
foreach(entry IN LISTS dirent_list)
    # 检查是否是目录
    if(IS_DIRECTORY ${entry})
        # 添加子目录
        add_subdirectory(${entry})
    endif(IS_DIRECTORY ${entry})
endforeach(entry IN LISTS dirent_list)
