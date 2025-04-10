#include "utils/tools.h"
#include "utils/logger.h"

#include <algorithm>
#include <cctype>  // For std::tolower
#include <dlfcn.h> // for loading dll

std::string string_tolower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);  // or [](unsigned char c){ return std::tolower(c); }
    return str;
}

std::string string_toupper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);  // or [](unsigned char c){ return std::tolower(c); }
    return str;
}

socket_handler::socket_handler(const char* _dp, const char* _sn) :
    dll_path(_dp),
    symbol_name(_sn),
    dll_h(nullptr),
    func_ptr(nullptr) {
    // 加载共享库
    // 即使多次调用 dlopen，只会加载一份副本到内存中
    dll_h = dlopen(dll_path, RTLD_LAZY); // 将 "./libmydll.so" 替换为你的共享库文件名
    // RTLD_LAZY: 延迟绑定，只有在函数第一次被调用时才解析地址
    // RTLD_NOW: 立即绑定，在 dlopen 返回之前解析所有未定义的符号

    if (dll_h == nullptr)
        ERRO("failed to load shared library: ", dlerror());
    // dlopen 和 dlsym 的成功只意味着动态库被成功加载，并且找到了具有指定名称的符号。
    // 这并不意味着类型匹配。
    func_ptr = (handler_ptr)dlsym(dll_h, symbol_name);

    if (func_ptr == nullptr)
    {
        ERRO("failed to get symbol address: ", dlerror());
        dlclose(dll_h); // 别忘了卸载共享库
        dll_h = nullptr;
    }

    INFO("plugin", dll_path, "loaded");
}

socket_handler::~socket_handler()
{
    // dlclose 的作用： dlclose 的作用是减少动态库的引用计数。
    // 只有当引用计数变为 0 时，系统才会真正卸载该动态库。
    if (dll_h != nullptr)
    {
        dlclose(dll_h);
        INFO("plugin", dll_path, "unloaded");
    }
}

int socket_handler::operator()(int sock)
{
    if (!this->is_valid())
    {
        ERRO("plugin not loaded properly", dll_path, symbol_name);
        throw std::runtime_error("empty plugin");
    }
    
    return func_ptr(sock);
}
