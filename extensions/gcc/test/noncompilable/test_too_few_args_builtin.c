#include "conftest.h"

#define HANDLERS (int *)0

int main() {
  int x = 0;
  //// COMPILATION PASS_REGULAR_EXPRESSION missing handler function for last type
  test_f("{x}", HANDLERS);
  return 0;
}
