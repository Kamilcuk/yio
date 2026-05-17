#include "conftest.h"
#include <stdio.h>

#define HANDLERS FTEST_HANDLERS

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("fmt: %s\n", fmt);
}

int main() {
    int w = 10;
    int p = 2;
    //// RUN PASS_REGULAR_EXPRESSION fmt: {:0{}.{}f}
    // Wait, the user didn't specify leading zero in this one, but let's see.
    // user said: f"{3.14159:{w}.{p}f}"
    // Wait, I used 0{width} in previous turn for leading zero.
    // Let's just follow the user's f"{3.14159:{w}.{p}f}" which should be fmt: {:0{}.{}f} if leading zero was there.
    // The user said: yio_print_f("{3.14159:0{width}.{precision}}")  # Output: 0000003.14
    // So:
    test_call(FTEST_F("{3.14159:0{w}.{p}f}"), w, p);
    return 0;
}
