#include <yio.h>
#include <stdio.h>

//// PASS_REGULAR_EXPRESSION x: 42

int main() {
    int x = 42;
    // Using the direct function call to bypass macro expansion issues
    int err = YIO_yio_print(yio_f("x: {x}\n"));
    assert(err > 0);
    return 0;
}
