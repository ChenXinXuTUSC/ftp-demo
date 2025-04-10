#include "common.h"
#include "utils/tools.h"

#include <dlfcn.h>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("usage: %s <dll_path> <func_name>\n", argv[0]);
        return -1;
    }

    socket_handler test_handler(argv[1], argv[2]);

    // 3. 调用函数
    int result = test_handler(10);
    std::cout << "result: " << result << std::endl;

    return 0;
}
