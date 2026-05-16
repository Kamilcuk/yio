#include "conftest.h"

FSTRING_FORMAT(1) static void print(const char *fmt, ...) {}

int main() {
    int x = 42;
    double f = 1.23;
    // 1. d as fill character for d
    print("{:d<10d}", x);
    // 2. Complex format string with all options
    print("{:*>+10.5d}", x);
    print("{:_^+20.10f}", f);
    return 0;
}
