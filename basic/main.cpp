//
// Created by binlee on 2020/8/30.
//

#include "main.h"
#include "src/cpp/cpp_string.h"
#include "src/cpp/Person.h"

#include <iostream>

void cpp_main();

int main() {
    // const int ret = c_main();
    // return ret;
    std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<" << std::endl;

    cpp_main();

    return 0;
}

void cpp_main() {
    cpp_string();

    person_main();
}
