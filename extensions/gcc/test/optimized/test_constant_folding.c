#include "conftest.h"

//// RUN PASS_REGULAR_EXPRESSION FMT: 0042
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: 0[xX]2A
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: \+42
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT:   42
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT:  42 
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: 42  
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0

void test_case_impl(ftest_handler *handlers, const char *fmt) {
    printf("FMT: %s\n", fmt);
    int count = 0;
    if (handlers) {
        while (handlers[count] != NULL) count++;
    }
    printf("HANDLERS: %d\n", count);
}

#define test_case(str) test_case_impl(test_f(str, FTEST_HANDLERS))

int main() {
    test_case("{42:04d}");
    test_case("{42:#X}");
    test_case("{42:+d}");
    test_case("{42:4d}");
    test_case("{42:^4d}");
    test_case("{42:<4d}");
    return 0;
}
