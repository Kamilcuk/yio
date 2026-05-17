#include <yio.h>
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION val: 0000003.14

int main() {
    double f = 3.14159;
    int width = 10;
    int precision = 2;
    int err = yio_print_f("val: {f:0{width}.{precision}f}\n");
    assert(err > 0);
    return 0;
}
