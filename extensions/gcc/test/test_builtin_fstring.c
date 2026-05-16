#include "conftest.h"
#include <stdio.h>

// Mock handlers
static int h_int(ftest_ctx* ctx) { printf("h_int called\n"); return 0; }
static int h_float(ftest_ctx* ctx) { printf("h_float called\n"); return 0; }
static int h_str(ftest_ctx* ctx) { printf("h_str called\n"); return 0; }

#define HANDLERS (int*)0, h_int, (float*)0, h_float, (const char**)0, h_str

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("test_call fmt: %s\n", fmt);
    for (int i = 0; handlers[i]; i++) handlers[i](NULL);
}

int main() {
    int x = 1;
    float y = 2.0f;
    const char *z = "3";
    test_call(test_f("{x}, {y}, {z}", HANDLERS));
    return 0;
}
// test_PASS_REGULAR_EXPRESSION test_call fmt: {}, {}, {}
// test_PASS_REGULAR_EXPRESSION h_int called
// test_PASS_REGULAR_EXPRESSION h_float called
// test_PASS_REGULAR_EXPRESSION h_str called
