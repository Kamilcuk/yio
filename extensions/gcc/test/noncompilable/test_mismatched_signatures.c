#include "conftest.h"

static int h1(ftest_ctx *ctx) { return 0; }
static int h2(int x) { return 0; }

#define HANDLERS (int *)0, h1, (float *)0, h2

int main() {
  int x = 0;
  //// COMPILATION PASS_REGULAR_EXPRESSION all handlers must have the same type in ‘__builtin_fstring’ (mismatched ‘int(int)’ vs ‘int(ftest_ctx *)’)
  test_f("{x}", HANDLERS);
  return 0;
}
