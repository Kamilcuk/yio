#include "conftest.h"

//// RUN PASS_REGULAR_EXPRESSION FMT: 42 and 123
//// RUN PASS_REGULAR_EXPRESSION HANDLERS: 0

void test_case_impl(ftest_handler *handlers, const char *fmt, ...) {
    printf("FMT: %s\n", fmt);
    int count = 0;
    if (handlers) {
        while (handlers[count] != NULL) count++;
    }
    printf("HANDLERS: %d\n", count);
    
    va_list va;
    va_start(va, fmt);
    for (int i = 0; i < count; ++i) {
        handlers[i](&va);
    }
    va_end(va);
}

// test_f(str, ...) provides handlers and the trailing "", which the plugin replaces with (handler_array, new_fmt).
// The runtime arguments (like runtime_val) should follow.
#define test_case(str, ...) test_case_impl(test_f(str, FTEST_HANDLERS), ##__VA_ARGS__)

int main() {
    int runtime_val = 123;
    // 42 is folded, runtime_val is not.
    test_case("{42} and {runtime_val}", runtime_val);
    return 0;
}
