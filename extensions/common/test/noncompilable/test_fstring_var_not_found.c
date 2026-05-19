#include "conftest.h"

// Builtin prototype
ftest_handler *__builtin_fstring(const char *str, ...);

#undef test_f
#define test_f(str, ...) __builtin_fstring(str, __VA_ARGS__)

static int h_int(ftest_ctx *ctx) { return 0; }
#define HANDLERS (int *)0, h_int

void test_call(ftest_handler handlers[], const char *fmt, ...) __attribute__((test_format(2)));
void test_call(ftest_handler handlers[], const char *fmt, ...) {}

int main() {
  // 1. Variable not found
  //// COMPILATION PASS_REGULAR_EXPRESSION variable or literal ‘non_existent’ not found
  test_call(test_f("val: {non_existent}", HANDLERS), "");
  return 0;
}
