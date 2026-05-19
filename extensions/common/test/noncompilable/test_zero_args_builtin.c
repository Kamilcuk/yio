#include "conftest.h"

int main() {
  //// COMPILATION PASS_REGULAR_EXPRESSION too few arguments to ‘__builtin_fstring’
  __builtin_fstring();
  return 0;
}
