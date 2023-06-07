//
// Created by binlee on 2020/8/30.
//

#include "mem_manage.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"

void memory_manage() {
  char name[100];
  char *desc;
  strcpy(name, "Benny Lee");
  // 动态分配内存
  if ((desc = malloc(100 * sizeof(char))) == NULL) {
    fprintf(stderr, "Error - unable to allocate required memory.\n");
  } else {
    strcpy(desc, "I studied english in Wall Street English 5 months ago.");
  }
  printf("name is %s\n", name);
  printf("desc is %s\n", desc);
  // 重新分配内存
  if ((desc = realloc(desc, 150 * sizeof(char))) == NULL) {
    fprintf(stderr, "Error - unable to realloc required memory.\n");
  } else {
    strcat(desc, " But now I quit my courses.");
  }
  printf("after realloc, desc is %s\n", desc);

  // 释放内存
  free(desc);
}