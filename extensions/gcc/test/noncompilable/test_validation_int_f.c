#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier.*f.*for.*int
int main() {
    int x = 42;
    print("{:f}", x);
    return 0;
}
