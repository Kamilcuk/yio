#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

int main() {
    int x = 42;
    int w = 10;
    int p = 5;
    ftest_call(test_f("val: {x:{w}d}", HANDLERS, (int*)0, h_int));
    ftest_call(test_f("val: {x:.{p}f}", HANDLERS, (int*)0, h_int));
    return 0;
}
