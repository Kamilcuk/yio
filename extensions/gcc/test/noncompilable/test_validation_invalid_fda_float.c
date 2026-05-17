#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specification

int main() {
    double f = 1.23;
    print("{:fda<10}", f);
    return 0;
}
