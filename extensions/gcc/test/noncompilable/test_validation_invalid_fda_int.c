#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specification

int main() {
    int i = 42;
    print("{:fda<10}", i);
    return 0;
}
