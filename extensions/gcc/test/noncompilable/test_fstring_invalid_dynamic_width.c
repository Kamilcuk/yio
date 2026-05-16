#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

//// COMPILATION PASS_REGULAR_EXPRESSION variable.*=.*not found
int main() {
    int x = 42;
    ftest_call(test_f("{x:{=}}", HANDLERS));
    return 0;
}
