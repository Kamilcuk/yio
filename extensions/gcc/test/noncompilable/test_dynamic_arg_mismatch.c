#include "../conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

int main() {
    float f = 3.14;
    
    // Missing width
    //// COMPILATION PASS_REGULAR_EXPRESSION too few arguments for format string \(expected 2, got 1\)
    print("{:{}f}", f);

    // Missing precision
    //// COMPILATION PASS_REGULAR_EXPRESSION too few arguments for format string \(expected 3, got 2\)
    print("{:{}.{}f}", f, 10);

    // Extra arg
    //// COMPILATION PASS_REGULAR_EXPRESSION too many arguments for format string \(expected 2, got 3\)
    print("{:{}f}", f, 10, 20);

    // Manual indexing mismatch
    //// COMPILATION PASS_REGULAR_EXPRESSION too few arguments for format string \(expected 3, got 2\)
    print("{0:{1}.{2}f}", f, 10);

    return 0;
}
