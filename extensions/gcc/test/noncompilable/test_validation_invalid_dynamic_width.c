#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

//// COMPILATION PASS_REGULAR_EXPRESSION invalid dynamic field index.*=
int main() {
    print("{:{=}}", 42, 10);
    return 0;
}
