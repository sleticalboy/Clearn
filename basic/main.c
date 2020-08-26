//
// Created by binlee on 20-7-31.
//

#include "str/str_usage.h"
#include "data_type/data_type.h"

int main() {

    printf("Hello, C World! This is my first c program!\n\n");

    printf(">>>>>>>>>>>start of string api\n");
    const char *input = "Hello, World!";
    printf("str len: %d\n", str_len(input));
    printf("str locate: %s\n", str_locate(input, 'd'));
    printf(">>>>>>>>>>>>>end of string api\n\n");

    printf(">>>>>>>>>>>start of fun basic()\n");
    basic();
    printf(">>>>>>>>>>>end of fun basic()\n");
    return 0;
}