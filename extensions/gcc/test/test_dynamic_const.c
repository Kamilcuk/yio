#include "conftest.h"
#include <stdio.h>

#define HANDLERS FTEST_HANDLERS

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("fmt: %s\n", fmt);
}

int main() {
    const int w = 10;
    const int p = 2;
    //// RUN PASS_REGULAR_EXPRESSION fmt: {:10.2f}
    test_call(FTEST_F("{3.14159:{w}.{p}f}"));
    return 0;
}
