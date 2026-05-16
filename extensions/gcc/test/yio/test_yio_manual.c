#include <yio.h>
#include <stdio.h>

//// PASS_REGULAR_EXPRESSION x: 42

int main() {
    int x = 42;
    // Using the direct function call to bypass macro expansion issues
    YIO_yio_print(yio_f("x: {x}\n"));
    return 0;
}
