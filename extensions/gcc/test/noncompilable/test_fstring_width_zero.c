#include "conftest.h"

//// COMPILATION PASS_REGULAR_EXPRESSION width cannot be negative
FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

int main() {
    print("{0:{1}}", 42, -1);
    return 0;
}
