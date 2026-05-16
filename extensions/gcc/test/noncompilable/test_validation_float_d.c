#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier d

int main() {
    double f = 1.23;
    print("{:d}", f);
    return 0;
}
