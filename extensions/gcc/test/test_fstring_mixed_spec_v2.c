#include "conftest.h"
#include <stdio.h>

#define HANDLERS FTEST_HANDLERS

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("fmt: %s\n", fmt);
}

int main() {
    float f = 3.14159;
    volatile int w = 10;
    volatile int p = 2;

    //// RUN PASS_REGULAR_EXPRESSION fmt: {:10.2f}
    //// RUN PASS_REGULAR_EXPRESSION h_double: 3.141590
    FTEST_PRINT_F("{3.14159:10.2f}");

    //// RUN PASS_REGULAR_EXPRESSION fmt: {:10.{}f}
    //// RUN PASS_REGULAR_EXPRESSION h_double: 3.141590
    //// RUN PASS_REGULAR_EXPRESSION h_int: 2
    FTEST_PRINT_F("{3.14159:10.{p}f}");

    //// RUN PASS_REGULAR_EXPRESSION fmt: {:{}f}
    //// RUN PASS_REGULAR_EXPRESSION h_double: 3.141590
    //// RUN PASS_REGULAR_EXPRESSION h_int: 10
    FTEST_PRINT_F("{3.14159:{w}f}");

    //// RUN PASS_REGULAR_EXPRESSION fmt: {:{}.{}f}
    //// RUN PASS_REGULAR_EXPRESSION h_double: 3.141590
    //// RUN PASS_REGULAR_EXPRESSION h_int: 10
    //// RUN PASS_REGULAR_EXPRESSION h_int: 2
    FTEST_PRINT_F("{3.14159:{w}.{p}f}");

    return 0;
}
