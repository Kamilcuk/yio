#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specification
int main() {
    print("{:???}", 1);
    return 0;
}
