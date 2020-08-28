//
// Created by binlee on 20-7-31.
//

#include <stdio.h>

#include "main/str_type.h"
#include "main/data_type.h"
#include "main/ptr_type.h"
#include "main/struct_type.h"
#include "main/macro_type.h"

int main() {

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
    return 0;
}