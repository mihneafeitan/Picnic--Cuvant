#include "../include/Utils.h"
#include <cassert>
#include <iostream>

int main() {
    assert(Utils::toLower("HELLO") == "hello");
    assert(Utils::isAlphaOnly("abc"));
    std::cout << "Test OK!\n";
    return 0;
}
