//
// Created by binlee on 20-8-2.
//

#include <string.h>
#include <stdio.h>
#include "str_type.h"

void str_api() {

  const char *input = "Hello C world!";
  // 计算字符长度
  printf("input: %s, len: %lu\n", input, strlen(input));

  const char target = 'C';
  // 定位字符在字符串中的位置
  printf("input: %s, target: %c, search: %s\n", input, target, strchr(input, target));

  // C语言中字符数组以 '\0' 结尾
  char greetings[9] = {'I', '\'', 'm', ' ', 'h', 'e', 'r', 'e', '\0'};
  printf("input is %s\n", greetings);

  char empty[100];

  // 字符串拷贝：从 input 拷贝到 empty
  strcpy(empty, input);
  printf("copy %s to empty -> %s\n", input, empty);

  // 字符串比较
  int result = strcmp(input, greetings);
  printf("%s compares to %s is %d\n", input, greetings, result);

  // 字符串拼接：把 greetings 拼接到 empty 后面
  strcat(empty, greetings);
  printf("after strcat, input is %s\n", empty);
}
