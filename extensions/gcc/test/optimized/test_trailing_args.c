#include "conftest.h"
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION FMT: 42
//// RUN PASS_REGULAR_EXPRESSION TRAILING: 456

void test_call_extra_impl(ftest_handler *handlers, const char *fmt, int extra) {
    printf("FMT: %s\n", fmt);
    printf("TRAILING: %d\n", extra);
}

#define test_call_extra(str, extra) test_call_extra_impl(test_f(str, FTEST_HANDLERS), extra)

int main() {
    test_call_extra("{42}", 456);
    return 0;
}
