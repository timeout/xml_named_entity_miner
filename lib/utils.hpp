#pragma once
#include <string>

namespace Utils {
    auto ltrim(std::string& str) -> std::string&;
    auto rtrim(std::string& str) -> std::string&;
    auto trim(std::string& str) -> std::string&;
}
