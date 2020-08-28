//
// Created by binli on 2020/8/28.
//

#include "macro_type.h"

void macro_main() {
    message(C, C++);
    printf("power 3 is %d\n", POWER(3));
    printf("file is %s\n", __FILE__);
    printf("file name is %d\n", __LINE__);
}