//
// Created by binli on 2020/8/28.
//

#include "macro_type.h"

void macro_main() {
  MSG(C, C++);
  printf("power 3 is %d\n", POWER(3));
  printf("the max between 15 and 9 is %d\n", MAX(15, 9));
  printf("file is %s\n", __FILE__);
  printf("this code in file is line %d\n", __LINE__);
  printf("date and time %s %s\n", __DATE__, __TIME__);
  printf("function name is %s\n", __func__);

  int arr[] = {0, 2, 5, 6, 9};
  printf("array length: %lu\n", ARR_LEN(arr));
}