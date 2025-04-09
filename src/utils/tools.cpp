#include "utils/tools.h"
#include <algorithm>
#include <cctype>  // For std::tolower

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
