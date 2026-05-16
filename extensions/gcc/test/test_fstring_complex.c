#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
static int h_double(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int, (double*)0, h_double

int main() {
    int x = 42;
    double f = 1.23;
    ftest_call(test_f("{x:d<10d}", HANDLERS));
    ftest_call(test_f("{x:*>+10.5d}", HANDLERS));
    ftest_call(test_f("{f:_^+20.10f}", HANDLERS));
    return 0;
}
