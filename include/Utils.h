#pragma once
#include <string>
#include <algorithm>

class Utils {
public:
    static std::string toLower(const std::string& s);
    static bool isAlphaOnly(const std::string& s);
};
