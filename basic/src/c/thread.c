//
// Created by binli on 2020/11/10.
//

#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int counter;

void *task() {
  while (counter >= 0) {
    usleep(300);
    printf("sub thread: %d\n", counter--);
  }
  return NULL;
}

void thread_main() {
  printf("%s() run...\n", __func__);
  counter = 20;
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, task, NULL);
  printf("%s() %d, tid: %lu\n", __func__, ret, tid);
  // while (counter > 0) {
  //   usleep(300);
  //   printf("main thread: %d\n", counter--);
  // }
  printf("%s() waiting sub thread exit\n", __func__);
  pthread_join(tid, NULL);
  printf("%s() exit\n", __func__);
}
