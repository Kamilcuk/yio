//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier f
#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

int main() {
    int x = 42;
    // Should fail if validation is implemented for f-strings
    ftest_call(test_f("{x:f}", HANDLERS));
    return 0;
}
