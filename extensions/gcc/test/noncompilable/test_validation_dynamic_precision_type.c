#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION dynamic precision argument 1 must be an integer
int main() {
    print("{:.{}f}", 1.2, 1.2);
    return 0;
}
