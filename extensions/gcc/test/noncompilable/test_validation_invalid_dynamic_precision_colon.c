#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION nested format specifier not allowed
int main() {
    print("{:.{:}}", 1.23, 10);
    return 0;
}
