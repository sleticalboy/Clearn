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
int calc_item(int level);

void quick_sort_impl(int left, int right);

void fibonacci(int level) {
    printf("the following is %d items of fibonacci sequence.\n", level);
    for (int i = 0; i <= level; ++i) {
        printf("%d ", calc_item(i));
    }
    printf("\n");
}

int calc_item(int level) {
    if (level == 0 || level == 1) {
        return level;
    }
    return calc_item(level - 1) + calc_item(level - 2);
}

void quick_sort() {
    printf("%s() run", __FUNCTION__);

    int arr[] = {5, 9, 3, 0, 1, 4, 2, 8, 6, 7, 10};

    int pivot = arr[0];
    int size = (sizeof(arr) / sizeof(pivot));
    int left = 0, right = size - 1;
    int tmp;

    printf("pivot: %d, left: %d, right: %d\n", pivot, left, right);

    while (left != right) {
        while (arr[right] >= pivot && left < right) {
            right--;
        }
        while (arr[left] <= pivot && left < right) {
            left++;
        }
        if (left < right) {
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
        }
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

void quick_sort_impl(int left, int right) {

}