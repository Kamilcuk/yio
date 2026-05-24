#include "conftest.h"

static int h_void(ftest_ctx *ctx) { return 0; }

int main() {
    //// COMPILATION PASS_REGULAR_EXPRESSION cannot use void as a target type for handler
    __builtin_fstring("{}", (void *)0, h_void);
    return 0;
}
