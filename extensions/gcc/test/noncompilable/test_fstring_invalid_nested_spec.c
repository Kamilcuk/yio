#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

//// COMPILATION PASS_REGULAR_EXPRESSION nested format specifier not allowed
int main() {
    int x = 42;
    int w = 10;
    ftest_call(test_f("{x:{w:f}}", HANDLERS));
    return 0;
}
