//
// Created by binlee on 2020/8/30.
//

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "variable_args.h"

/**
 * 可变参数
 * @param ...
 */
void variable_args_int(int num, ...);

void variable_args_str(int num, ...);

void variable_args_main() {
    variable_args_int(3, 4, 5, 6);
    variable_args_str(4, "Hello", "World", "C/C++", "Java");
}

void variable_args_int(int num, ...) {
    va_list args;
    double sum = 0.0;
    /* 为 num 个参数初始化 args */
    va_start(args, num);
    for (int i = 0; i < num; ++i) {
        /* 逐个访问赋给 args 的参数 */
        int arg = va_arg(args, int);
        printf("arg %d is %d\n", i, arg);
        sum += arg;
    }
    /* 清理为 args 保留的内存 */
    va_end(args);
    printf("args sum is %f\n", sum);
}

void variable_args_str(int num, ...) {
    va_list args;
    va_start(args, num);
    char total[100];
    for (int i = 0; i < num; ++i) {
        char *str = va_arg(args, char*);
        printf("arg %d is %s\n", i, str);
        strcat(total, str);
    }
    va_end(args);
    printf("total is %s\n", total);
}
