#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void w_assert_fail(const char *file, int line, const char *statement) {
    printf("Assert Fail %s:%d %s\n", file, line, statement);
    exit(1);
}
