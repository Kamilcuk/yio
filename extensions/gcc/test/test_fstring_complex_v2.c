#include "conftest.h"
#include <stdint.h>
#include <stdio.h>

static int h_int(ftest_ctx *ctx) {
  if (ctx) printf("h_int: %d\n", *(int *)ctx);
  else printf("h_int called\n");
  return 0;
}

static int h_float(ftest_ctx *ctx) {
  if (ctx) printf("h_float: %f\n", *(float *)ctx);
  else printf("h_float called\n");
  return 0;
}

#define MY_HANDLERS (int *)0, h_int, (float *)0, h_float

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    printf("fmt: %s\n", fmt);
}

int main() {
    int val = 42;
    int w = 10;
    int p = 2;
    float f = 3.14159;

    // 1. Multiple dynamic fields
    // RUN: fmt: {:{}f} {:{}d}
    test_call(test_f("{f:{}f} {val:{}d}", MY_HANDLERS), f, w, val, w);

    // 2. Mixed literal and dynamic
    // RUN: fmt: {:10.{}f}
    test_call(test_f("{f:10.{}f}", MY_HANDLERS), f, p);
    
    // RUN: fmt: {:{}.5f}
    test_call(test_f("{f:{}.5f}", MY_HANDLERS), f, w);

    // 3. Positional in dynamic fields (if we decide to support it in __builtin_fstring)
    // Wait, currently expand_fstring_builtin doesn't support numeric indices in __builtin_fstring.
    // But validate_fstring_format does.

    return 0;
}
