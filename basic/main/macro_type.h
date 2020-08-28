//
// Created by binli on 2020/8/28.
//

#include <stdio.h>

// 定义宏
#define message(a, b) \
    printf(#a " and " #b ": we love you!\n")

#define POWER(num) (num * num)

void macro_main();