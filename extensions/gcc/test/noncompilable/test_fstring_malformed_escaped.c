#include "conftest.h"

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

//// COMPILATION PASS_REGULAR_EXPRESSION unmatched.*}
int main() {
    ftest_call(test_f("{{}:}", HANDLERS));
    return 0;
}
