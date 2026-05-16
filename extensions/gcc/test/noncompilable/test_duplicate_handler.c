#include "conftest.h"

static int h_int1(ftest_ctx* ctx) { return 0; }
static int h_int2(ftest_ctx* ctx) { return 0; }

#define HANDLERS (int*)0, h_int1, (int*)0, h_int2

int main() {
    int x = 0;
    //// COMPILATION PASS_REGULAR_EXPRESSION duplicate handler for type ‘int’
    test_f("{x}", HANDLERS);
    return 0;
}
