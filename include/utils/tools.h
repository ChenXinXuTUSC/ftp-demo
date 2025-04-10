#ifndef __TOOLS__
#define __TOOLS__

#include <string>

std::string string_tolower(std::string str);
std::string string_toupper(std::string str);


class socket_handler
{
    typedef int (*handler_ptr)(int);

public:
    socket_handler() = delete;
    socket_handler(const char* dll_path, const char* symbol_name);
    ~socket_handler();

    int operator()(int sock);

private:
    const char* dll_path;
    const char* symbol_name;
    void* dll_h;
    handler_ptr func_ptr;
};


#endif
