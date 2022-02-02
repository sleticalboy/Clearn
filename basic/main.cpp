//
// Created by binlee on 2020/8/30.
//

#include "main.h"
#include "src/cpp/cpp_string.h"
#include "src/cpp/Person.h"

#include <iostream>

int cpp_main();

int main() {
    // c 语言
    const int ret = c_main();
    if (ret) return ret;

    // cpp
    std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<" << std::endl;
    return cpp_main();
}

int cpp_main() {
    cpp_string();

    person_main();

    return 0;
}
