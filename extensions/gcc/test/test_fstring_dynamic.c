#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

int main() {
    int x = 42;
    int w = 10;
    int p = 5;
    //// RUN PASS_REGULAR_EXPRESSION fmt: val: {:{}d}.*h_int: 42.*h_int: 10
    ftest_call(FTEST_F("val: {x:{w}d}"));
    //// RUN PASS_REGULAR_EXPRESSION fmt: val: {:.{}d}.*h_int: 42.*h_int: 5
    ftest_call(FTEST_F("val: {x:.{p}d}"));
    return 0;
}
