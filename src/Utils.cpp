#include "../include/Utils.h"

std::string Utils::toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return out;
}

bool Utils::isAlphaOnly(const std::string& s) {
    return std::all_of(s.begin(), s.end(),
                       [](unsigned char c){ return std::isalpha(c); });
}
