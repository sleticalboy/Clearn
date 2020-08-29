//
// Created by binli on 2020/8/28.
//

#include <stdio.h>

#ifndef MSG
// 定义宏， 其中 # 叫做字符串常量化运算符，在将宏参数转换成字符串是才会用到
#define MSG(a, b) \
    printf(#a " and " #b ": we love you!\n")
#endif

#ifndef POWER
// 定义宏：计算一个数的平方
#define POWER(num) ((num) * (num))
#endif

#ifndef MAX
// 定义宏：求两数中的最大值
#define MAX(x, y) (x > y ? x : y)
#endif

void macro_main();