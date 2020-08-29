//
// Created by binli on 2020/8/28.
//

#include "macro_type.h"

void macro_main() {
    MSG(C, C++);
    printf("power 3 is %d\n", POWER(3 + 3));
    printf("the max between 13 and 9 is %d\n", MAX(13 + 2, 9));
    printf("file is %s\n", __FILE__);
    printf("this code in file is line %d\n", __LINE__);
    printf("date and time %s %s\n", __DATE__, __TIME__);
}