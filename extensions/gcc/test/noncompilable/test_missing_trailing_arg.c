#include "conftest.h"

int main() {
  int x = 0;
  // Missing the trailing "" argument entirely
  //// COMPILATION PASS_REGULAR_EXPRESSION must be an empty string literal
  ftest_call((ftest_handler *)__builtin_fstring("val: {x}", (int *)0, (ftest_handler)0), "placeholder");
  return 0;
}
