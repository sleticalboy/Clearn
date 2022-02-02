//
// Created by binli on 2020/11/10.
//

#include "thread.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t pid;
int counter;

void *task() {
    while (counter > 0) {
        usleep(300);
        printf("sub thread running...%d\n", counter--);
    }
    return NULL;
}

void thread_main() {
    printf("pthread_main() run...\n");
    counter = 20;
    int ret = pthread_create(&pid, NULL, task, NULL);
    printf("pthread_create %d, pid: %lu\n", ret, pid);
    while (counter > 0) {
        usleep(300);
        printf("main thread running...%d\n", counter--);
    }
}
