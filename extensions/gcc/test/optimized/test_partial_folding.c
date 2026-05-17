#include "conftest.h"
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION FMT: {:10d}
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 1

void test_case_impl(ftest_handler *handlers, const char *fmt) {
    printf("FMT: %s\n", fmt);
    int count = 0;
    if (handlers) {
        while (handlers[count] != NULL) count++;
    }
    printf("HANDLERS: %d\n", count);
}

int main() {
    volatile int vx = 123;
    test_case_impl(test_f("{vx:{10}d}", FTEST_HANDLERS));
    return 0;
}
