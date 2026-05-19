#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION too few arguments

int main() {
    print("TEST {} {}", 1);
    return 0;
}
