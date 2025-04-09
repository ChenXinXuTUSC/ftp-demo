file(
    GLOB_RECURSE utils_list
    *.cpp
)

add_library(utils SHARED ${utils_list})
