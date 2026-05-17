#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION dynamic width argument 1 must be an integer
int main() {
    print("{:{}d}", 42, "string");
    return 0;
}
