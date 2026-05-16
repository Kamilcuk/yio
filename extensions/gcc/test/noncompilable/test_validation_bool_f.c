#include "conftest.h"
#include <stdbool.h>

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid format specifier.*f.*for.*int
void test_bool_f() {
    // In C, bool is promoted to int when passed to variadic function
    print("{:f}", true);
}

int main() {
    return 0;
}
