//
// Created by binli on 2020/8/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "ptr_type.h"

/**
 * 函数指针
 * @param a
 * @param b
 */
void fun_ptr(int a, int b) {
    printf("a: %d, b: %d, a + b: %d\n", a, b, a + b);
}

void ptr_type() {
    // pointer
    int a = 20;
    printf("a = %d, mem addr=%p\n", a, &a);

    int arr[5] = {1, 2, 3, 4, 5};
    // 计算数组长度
    int len = sizeof(arr) / sizeof(*arr/*arr[0]*/);
    printf("sizeof(arr) is %d\n", len);
    // 使用指针遍历数组
    printf("use pointer to print array:\n");
    for (int i = 0; i < len; ++i) {
        printf("%p -> %d ", &arr[i], *(arr + i));
    }
    printf("\n");
    // 使用索引遍历数组
    printf("use index to print array:\n");
    for (int i = 0; i < len; ++i) {
        printf("%d -> %d ", i, arr[i]);
    }
    printf("\n");
    // 空指针
    int *null_ptr = NULL;
    printf("null pointer is %p\n", null_ptr);
    // 空指针的内存地址是 0x00
    printf("null pointer's address is %lu\n", null_ptr);

    // 判断空指针
    if (null_ptr == 0) {
        printf("this is an null pointer.\n");
    } else {
        printf("this is not an null pointer.\n");
    }
    // 数组指针, 其元素指向每个 int 值的内存地址
    int *ptr_arr[len];
    for (int i = 0; i < len; ++i) {
        ptr_arr[i] = &arr[i];
    }
    for (int i = 0; i < len; ++i) {
        printf("ptr arr[%d] is %d\n", i, *ptr_arr[i]);
    }
    // 字符串本身就是一个 char* 指针
    // 字符串数组指针
    char *names[] = {"ben", "lynne", "will", "jordan"};
    len = sizeof(names) / sizeof(*names);
    printf("sizeof(*names): %lu, sizeof(char*): %lu, length of names: %d\n", sizeof(names), sizeof(char *), len);
    for (int i = 0; i < len; ++i) {
        printf("name[%d] is %s\n", i, names[i]);
    }

    // 函数指针: 指向同形参表函数的指针(数据类型要与函数返回值类型保持一直)
    void (*fun)(int, int) = fun_ptr;
    printf("call fun_ptr -->\n");
    fun(3, 5);
    fun(5, 8);
}

int generate(void) {
    return rand();
}

void fill_array(int *arr, size_t size, int (*generate)(void)) {
    for (size_t i = 0; i < size; ++i) {
        // 调用回调函数生成数组中的元素
        *(arr + i) = generate();
    }
}

void callback_fun() {
    int arr[10];
    size_t size = sizeof(arr) / sizeof(*arr);
    // 回调函数： generate() 可以参考 java 语言中的回调接口
    fill_array(arr, size, generate);
    for (size_t i = 0; i < size; ++i) {
        printf("arr[%zu] is %d\n", i, arr[i]);
    }
}
/*
 public class Main {

    public interface Generator {
        int generate();
    }

    public void fillArray(int[] arr, Generator generator) {
        for (int i = 0; i < arr.length; i++) {
            arr[i] = generator.generate();
        }
    }

    public static void main(String[] args) {
        final Main main = new Main();
        final int[] array = new int[10];
        main.fillArray(array, new Generator() {

            @Override
            public int generate() {
                return new Random().nextInt();
            }
        });
        for (int i = 0; i < array.length; i++) {
            System.out.println(String.format("arr[%d] is %d", i, array[i]));
        }
    }
 }
 */
