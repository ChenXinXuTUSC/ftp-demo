file(
    GLOB_RECURSE test_list
    *.cpp
)

foreach(test_path IN LISTS test_list)
    get_filename_component(test_name ${test_path} NAME_WE)
    add_executable(${test_name} ${test_path})
    target_link_libraries(${test_name} src)
endforeach(test_path IN LISTS test_list)
