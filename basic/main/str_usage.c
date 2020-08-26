//
// Created by binlee on 20-8-2.
//

#include <string.h>
#include "str_usage.h"

int str_len(const char *input) {
    return strlen(input);
}

char *str_locate(const char *input, char target) {
    return strchr(input, target);
}
