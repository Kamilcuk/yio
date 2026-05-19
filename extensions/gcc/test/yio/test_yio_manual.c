#include <yio.h>
#include <stdio.h>

//// RUN PASS_REGULAR_EXPRESSION x: 42

int main() {
    int x = 42;
    // Using the direct function call to bypass macro expansion issues
    int err = YIO_yio_print(YIO_F_("x: {x}\n"));
    assert(err > 0);
    return 0;
}
