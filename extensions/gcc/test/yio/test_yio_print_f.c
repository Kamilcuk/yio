#include <yio.h>
#include <stdio.h>

//// PASS_REGULAR_EXPRESSION x: 42

int main() {
    int x = 42;
    yio_print_f("x: {x}\n");
    return 0;
}
