//
// Created by binlee on 20-7-31.
//

#include <stdio.h>

#include "str/str_usage.h"

int main() {
    const char *input = "Hello, World!";
    printf("%s\n", input);

    printf("string api start >>>>>>>>>>>\n");
    printf("str len: %d\n", str_len(input));
    printf("str locate: %s\n", str_locate(input, 'd'));
    printf("string api end >>>>>>>>>>>>>\n");
    return 0;
}