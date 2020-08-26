//
// Created by binli on 2020/8/26.
//

#include <stdio.h>
#include "data_type.h"

void enum_type() {
    // 声明 enum 变量
    enum DAY day;
    day = FRI;
    printf("day: %d\n", day);
    // 只有连续的 enum 类型，才可以被这样遍历
    for (day = MON; day <= SUN; day++) {
        printf("day: %d\n", day);
    }
}

/**
 * c basic
 */
void basic() {
    printf("size of int: %lu\n", sizeof(int));
    printf("size of long: %lu\n", sizeof(long));
    printf("size of float: %lu\n", sizeof(float));
    printf("size of double: %lu\n", sizeof(double));
    printf("size of char: %lu\n", sizeof(char));
    printf("size of short: %lu\n", sizeof(short));
    printf("size of unsigned char: %lu\n", sizeof(unsigned char));
    printf("size of unsigned int: %lu\n", sizeof(unsigned int));
    printf("size of unsigned long: %lu\n", sizeof(unsigned long));
    // printf("size of unsigned double: %d\n", sizeof(unsigned double));
    // printf("size of unsigned float: %d\n", sizeof(unsigned float));
    printf("size of long int: %lu\n", sizeof(long int));
    printf("size of long long: %lu\n", sizeof(long long));
    printf("size of long double: %lu\n", sizeof(long double));
    // extern int a 与 int a 的区别：
    // extern：只声明，不开辟存储空间
    // 非 extern：声明&开辟存储空间
    printf("constant value LENGTH: %d\n", LENGTH);
    const int WIDTH = 50;
    printf("constant value WIDTH: %d\n", WIDTH);
    printf("area is WIDTH * LENGTH = %d\n", WIDTH * LENGTH);
}

