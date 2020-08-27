//
// Created by binlee on 20-7-31.
//

#include <stdio.h>

#include "main/str_type.h"
#include "main/data_type.h"
#include "main/ptr_type.h"
#include "main/struct_type.h"

int main() {

    printf("Hello, C World! This is my first c program!\n\n");

    printf(">>>>>about data type>>>>>>\n");
    basic();
    enum_type();

    printf(">>>>>about pointer>>>>>>\n");
    ptr_type();
    callback_fun();

    printf(">>>>>about string>>>>>>\n");
    str_api();

    printf(">>>>>about struct>>>>>>\n");
    about_struct();
    return 0;
}