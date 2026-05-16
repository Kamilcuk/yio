#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

//// COMPILATION PASS_REGULAR_EXPRESSION variable.*=.*not found
int main() {
    double f = 1.23;
    ftest_call(test_f("{f:.{=}}", HANDLERS));
    return 0;
}
