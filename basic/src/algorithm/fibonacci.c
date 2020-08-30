//
// Created by binlee on 2020/8/29.
//

#include <stdio.h>
#include "fibonacci.h"

/**
 * 计算 item 值
 * @param level 第几项
 * @return
 */
int calc_item(int level) {
    if (level == 0 || level == 1) {
        return level;
    }
    return calc_item(level - 1) + calc_item(level - 2);
}

void fibonacci(int level) {
    printf("the following is %d items of fibonacci sequence.\n", level);
    for (int i = 0; i <= level; ++i) {
        printf("%d ", calc_item(i));
    }
    printf("\n");
}