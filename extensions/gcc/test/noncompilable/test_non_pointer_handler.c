#include "conftest.h"

#define HANDLERS (int *)0, 123

int main() {
  int x = 0;
  //// COMPILATION PASS_REGULAR_EXPRESSION handler must be a pointer or function in ‘__builtin_fstring’
  test_f("{x}", HANDLERS);
  return 0;
}
