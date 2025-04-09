#include "utils/tools.h"
#include <iostream>

int main()
{
    std::string myString    = "Hello World";
    std::string lowerString = string_tolower(myString);
    std::cout << "Original string: " << myString << std::endl;
    std::cout << "Lowercase string: " << lowerString << std::endl;
    return 0;
}
