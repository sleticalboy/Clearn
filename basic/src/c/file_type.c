//
// Created by binlee on 2020/8/28.
//

#include <stdio.h>
#include <stdlib.h>
#include "file_type.h"

void print_current_file() {
  // __FILE__ 用于获取当前文件
  FILE *file = fopen(__FILE__, "r");
  int ch;
  do {
    ch = fgetc(file);
    putchar(ch);
  } while (ch != EOF);
  fclose(file);
}

void read_line(FILE *file, char *buf, int line) {
  if (file == NULL) {
    return;
  }

  int i = 1;
  while (!feof(file)) {
    if (i == line) {
      fgets(buf, (int) sizeof(char *) / sizeof(char), file);
      printf("line %d    %s\n", i, buf);
      break;
    }
    i++;
  }
}

void file_main() {

  print_current_file();

  const char *cmake = "../CMakeLists.txt";

  FILE *file;
  if ((file = fopen(cmake, "r")) == NULL) {
    printf("open file %s error!\n", cmake);
    return;
  }

  // char line[1024];
  // read_line(file, line, 4);

  printf("\nwe're going to print file %s\n", cmake);
  char buf[1024];
  int num = 1;
  while (!feof(file)) {
    fgets(buf, sizeof(buf) / sizeof(char), file);
    if (num <= 9) {
      printf("%d      %s", num, buf);
    } else {
      printf("%d     %s", num, buf);
    }
    num++;
  }

  fclose(file);
}