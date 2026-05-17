#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

int main() {
    int x = 42;
    //// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier.*f.*for.*int
    print("{:f}", x);
    return 0;
}
