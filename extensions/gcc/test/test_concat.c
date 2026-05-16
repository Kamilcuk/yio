#include "conftest.h"
#include <stdio.h>

static int h_int(ftest_ctx *ctx) { return 0; }

#define HANDLERS (int *)0, h_int

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) { printf("fmt: %s\n", fmt); }

int main() {
  int x = 42;
  // test literal merging
  printf("--- Concatenation ---\n");
  test_call(test_f(
      "val: {"
      "x"
      "}",
      HANDLERS
  ));
  return 0;
}

// test_PASS_REGULAR_EXPRESSION fmt: val: {}
