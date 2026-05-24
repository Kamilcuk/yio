#include "conftest.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION FMT: -2147483648
//// RUN PASS_REGULAR_EXPRESSION FMT: 18446744073709551615
//// RUN PASS_REGULAR_EXPRESSION FMT: 170141183460469231731687303715884105727

int test_case_impl(ftest_handler *handlers, const char *fmt, ...) {
    printf("FMT: %s\n", fmt);
    return 0;
}

static int ftest_h_ulong(ftest_ctx *ctx) {
    unsigned long val = va_arg(*ctx, unsigned long);
    printf("h_ulong: %lu\n", val);
    return 0;
}

static int ftest_h_i128(ftest_ctx *ctx) {
    // Just skip it for the mock
    va_arg(*ctx, __int128);
    printf("h_i128\n");
    return 0;
}

#define BOUNDARY_HANDLERS \
    FTEST_HANDLERS, \
    (unsigned long *)0, ftest_h_ulong, \
    (__int128 *)0, ftest_h_i128

int main() {
    const int i_min = INT_MIN;
    test_case_impl(test_f("{i_min}", BOUNDARY_HANDLERS));

    const uint64_t u64_max = 0xffffffffffffffffULL;
    test_case_impl(test_f("{u64_max}", BOUNDARY_HANDLERS));

    const __int128 i128_max = ((__int128)0x7fffffffffffffffULL << 64) | 0xffffffffffffffffULL;
    test_case_impl(test_f("{i128_max}", BOUNDARY_HANDLERS));

    return 0;
}
