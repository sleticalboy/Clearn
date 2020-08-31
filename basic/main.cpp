//
// Created by binlee on 2020/8/30.
//

#include "main.h"
#include "src/cpp/cpp_string.h"

#include <iostream>

using namespace std;

void cpp_main();

int main() {
    const int ret = c_main();
    cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<" << endl;

    cpp_main();

    return ret;
}

void cpp_main() {
    cpp_string();
}
