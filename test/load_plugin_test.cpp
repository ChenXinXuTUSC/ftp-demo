#include <dlfcn.h> // 包含动态链接库 API
#include <iostream>
#include <stdio.h>

// 定义函数指针类型
typedef int (*handler)(int); // 假设动态链接库中的函数是 int MyFunction(int);

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("usage: %s <dll_path> <func_name>\n", argv[0]);
        return -1;
    }

    // 1. 加载共享库
    void* handle = dlopen(argv[1], RTLD_LAZY); // 将 "./libmydll.so" 替换为你的共享库文件名
    // RTLD_LAZY: 延迟绑定，只有在函数第一次被调用时才解析地址
    // RTLD_NOW: 立即绑定，在 dlopen 返回之前解析所有未定义的符号

    if (handle == nullptr)
    {
        std::cerr << "failed to load shared library: " << dlerror() << std::endl;
        return 1;
    }

    // 2. 获取函数地址
    handler func =
        (handler)dlsym(handle, argv[2]); // 将 "MyFunction" 替换为你的函数名

    if (func == nullptr)
    {
        std::cerr << "failed to get function address: " << dlerror() << std::endl;
        dlclose(handle); // 别忘了卸载共享库
        return 1;
    }

    // 3. 调用函数
    int result = func(10);
    std::cout << "result: " << result << std::endl;

    // 4. 卸载共享库
    dlclose(handle);

    return 0;
}
