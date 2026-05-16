#include "conftest.h"

int main() {
  int x = 0;
  // Trailing argument is a non-empty string
  //// COMPILATION PASS_REGULAR_EXPRESSION must be an empty string literal
  ftest_call((ftest_handler *)__builtin_fstring("val: {x}", (int *)0, (ftest_handler)0), "invalid");
  return 0;
}
