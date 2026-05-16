#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION unmatched.*}
int main() {
    print("TEST }");
    return 0;
}
