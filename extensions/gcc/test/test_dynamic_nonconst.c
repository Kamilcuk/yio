#include "conftest.h"
#include <stdio.h>

#define HANDLERS FTEST_HANDLERS

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("fmt: %s\n", fmt);
}

int main() {
    volatile int w = 10;
    volatile int p = 2;
    //// RUN PASS_REGULAR_EXPRESSION fmt: {:0{}.{}f}
    test_call(FTEST_F("{3.14159:0{w}.{p}f}"), w, p);
    return 0;
}
