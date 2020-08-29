//
// Created by binlee on 2020/8/29.
//

#include <stdio.h>
#include "fibonacci.h"

int calc_single_level(int level) {
    if (level == 0 || level == 1) {
        return level;
    }
    return calc_single_level(level - 1) + calc_single_level(level - 2);
}

void fibonacci(int level) {
    printf("the following is %d items of fibonacci sequence.\n", level);
    for (int i = 0; i <= level; ++i) {
        printf("%d ", calc_single_level(i));
    }
}