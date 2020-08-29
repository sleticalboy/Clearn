//
// Created by binlee on 20-7-31.
//

#include <stdio.h>

#include "src/main/str_type.h"
#include "src/main/data_type.h"
#include "src/main/ptr_type.h"
#include "src/main/struct_type.h"
#include "src/main/macro_type.h"
#include "src/main/file_type.h"

#include "src/algorithm/fibonacci.h"

void basic_main();

void alg_main();

int main() {

    // basic_main();
    alg_main();

    return 0;
}

void alg_main() {
    fibonacci(13);
}

void basic_main() {
    printf("Hello, C World! This is my first c program!\n");

    printf("\n>>>>>about data type>>>>>>\n");
    basic();
    enum_type();

    printf("\n>>>>>about pointer>>>>>>\n");
    ptr_type();
    callback_fun();

    printf("\n>>>>>about string>>>>>>\n");
    str_api();

    printf("\n>>>>>about struct>>>>>>\n");
    about_struct();

    printf("\n>>>>>about macro>>>>>>\n");
    macro_main();

    printf("\n>>>>>about file>>>>>>\n");
    file_main();
}