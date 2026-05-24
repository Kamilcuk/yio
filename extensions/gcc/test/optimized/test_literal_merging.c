#include "conftest.h"
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION FMT: prefix_42_suffix
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0

int test_case_impl(ftest_handler *handlers, const char *fmt, ...) {
    printf("FMT: %s\n", fmt);
    int count = 0;
    if (handlers) {
        while (handlers[count] != NULL) count++;
    }
    printf("HANDLERS: %d\n", count);
    return 0;
}

int main() {
    test_case_impl(test_f("prefix_{42}_suffix", FTEST_HANDLERS));
    return 0;
}
