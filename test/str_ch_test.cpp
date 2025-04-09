#include <iostream>
#include <string>

int main()
{
    std::string str = "Hello";
    std::cout << "Original string: " << str << std::endl; // Output: Original string: Hello

    // 获取指向底层字符数组的指针
    char* data = str.data(); // Or use &str[0], but data() is safer in C++17 and later

    // 修改底层字符数组
    data[0] = 'J';
    data[1] = 'i';

    std::cout << "Modified string: " << str << std::endl; // Output: Modified string: Jillo

    return 0;
}
