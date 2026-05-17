#include "../conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

static int h_int(ftest_ctx *ctx) { return 0; }
#define HANDLERS (int*)0, h_int

int main() {
    int x = 42;
    
    // 1. Literal negative width in __builtin_fstring
    //// COMPILATION PASS_REGULAR_EXPRESSION width cannot be negative
    __builtin_fstring("{x:{-1}d}", (int*)0, h_int);

    // 2. Literal negative precision in __builtin_fstring
    //// COMPILATION PASS_REGULAR_EXPRESSION precision cannot be negative
    __builtin_fstring("{x:.{-5}f}", (int*)0, h_int);

    // 3. Negative argument for dynamic width in fstring_format
    //// COMPILATION PASS_REGULAR_EXPRESSION width cannot be negative
    print("{:{}d}", x, -1);

    // 4. Negative argument for dynamic precision in fstring_format
    //// COMPILATION PASS_REGULAR_EXPRESSION precision cannot be negative
    print("{:.{}f}", 3.14, -5);

    return 0;
}
