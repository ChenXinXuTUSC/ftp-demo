file(
    GLOB_RECURSE utils_list
    *.cpp
)

add_library(utils SHARED ${utils_list})
# if your gcc is old version, you may need to link against
# the thread library by yourself
#target_link_libraries(src pthread)
