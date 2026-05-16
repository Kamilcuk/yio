#include "conftest.h"

static int h_int(ftest_ctx *ctx) { return 0; }
static int h_mismatch(int x) { return 0; }

#define HANDLERS (int *)0, h_int, (float *)0, h_mismatch

int main() {
  int x = 0;
  //// COMPILATION PASS_REGULAR_EXPRESSION all handlers must have the same type in ‘__builtin_fstring’
  test_f("{x}", HANDLERS);
  return 0;
}
