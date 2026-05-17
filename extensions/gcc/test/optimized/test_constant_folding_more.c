#include "conftest.h"

//// RUN PASS_REGULAR_EXPRESSION FMT: 0042
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: 123
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: -123
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: 0b101010
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: 0o52
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT:   42
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0
//// RUN PASS_REGULAR_EXPRESSION FMT: {:6.2f}
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 1

void test_case_impl(ftest_handler *handlers, const char *fmt, ...) {
    printf("FMT: %s\n", fmt);
    int count = 0;
    if (handlers) {
        while (handlers[count] != NULL) count++;
    }
    printf("HANDLERS: %d\n", count);
}

#define test_case(str, ...) test_case_impl(test_f(str, FTEST_HANDLERS), ##__VA_ARGS__)

int main() {
    const int x = 42;
    test_case("{x:04d}");

    const long y = 123;
    test_case("{y}");
    
    const long long z = -123;
    test_case("{z}");

    test_case("{42:#b}");
    test_case("{42:#o}");
    
    // Constant width/precision folding
    test_case("{42:{4}d}");

    // Floats are NOT folded. 
    // The literal 1.2345 is not folded, so it remains as a {} with one handler.
    // The width {6} and precision {2} ARE folded into the spec.
    test_case("{1.2345:{6}.{2}f}", 1.2345);
    
    return 0;
}
