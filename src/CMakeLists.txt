add_subdirectory(utils)

file(
    GLOB_RECURSE src_list
    *.cpp
)

add_library(src SHARED ${src_list})
target_link_libraries(src utils)
# if your gcc is old version, you may need to link against
# the thread library by yourself
#target_link_libraries(src pthread)
