#include "common.h"
#include "utils/logger.h"

void my_terminate_handler()
{
    try
    {
        // 尝试获取未捕获的异常
        std::exception_ptr eptr = std::current_exception();
        if (eptr)
            std::rethrow_exception(eptr); // 重新抛出异常，以便可以捕获它
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown exception" << std::endl;
    }
    std::abort(); // 仍然需要调用 abort() 来终止程序
}
