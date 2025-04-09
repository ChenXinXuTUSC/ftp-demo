file(
    GLOB_RECURSE app_list
    *.cpp
)

foreach(app_path IN LISTS app_list)
    get_filename_component(app_name ${app_path} NAME_WE)
    add_executable(${app_name} ${app_path})
    target_link_libraries(${app_name} src)
endforeach(app_path IN LISTS app_list)
