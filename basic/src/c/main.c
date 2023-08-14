//
// Created by binlee on 20-7-31.
//

#include <stdio.h>

#include "str_type.h"
#include "data_type.h"
#include "ptr_type.h"
#include "struct_type.h"
#include "macro_type.h"
#include "file_type.h"
#include "variable_args.h"
#include "mem_manage.h"
#include "thread.h"

#include "algorithm/fibonacci.h"

#include "main.h"

void basic_main();

void alg_main();

int c_samples() {
    // 基础
    basic_main();
    // 算法
    alg_main();
    return 0;
}

void alg_main() {
    printf("\n>>>>>about algorithm>>>>>>\n");
    fibonacci(13);
    printf(">>>>>quick sort>>>>>>\n");
    quick_sort();
}

void basic_main() {
    printf("Hello, C World! This is my first c program!\n");

    printf("\n>>>>>about data type>>>>>>\n");
    basic();
    enum_type();

    printf("\n>>>>>about pointer>>>>>>\n");
    ptr_type();
    callback_fun();

    printf("\n>>>>>about string>>>>>>\n");
    str_api();

    printf("\n>>>>>about struct>>>>>>\n");
    about_struct();

    printf("\n>>>>>about union>>>>>>\n");
    about_union();

    printf("\n>>>>>about macro>>>>>>\n");
    macro_main();

    printf("\n>>>>>about file>>>>>>\n");
    file_main();

    printf("\n>>>>>about variable arguments>>>>>>\n");
    variable_args_main();

    printf("\n>>>>>about memory management>>>>>>\n");
    memory_manage();

    printf("\n>>>>>about thread usage>>>>>>\n");
    thread_main();
}

int main() {
  // c 语言
  return c_samples();
}