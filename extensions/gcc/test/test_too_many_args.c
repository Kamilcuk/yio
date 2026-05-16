#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION too many arguments

int main() {
    print("TEST {}", 1, 2);
    return 0;
}
