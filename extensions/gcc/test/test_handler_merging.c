#include "conftest.h"
#include <stdio.h>

static int h_int(ftest_ctx* ctx) { return 0; }
#define HANDLERS (int*)0, h_int

FSTRING_FORMAT(2)
static void test_call(ftest_handler handlers[], const char *fmt, ...) {
    static void* last_handlers = NULL;
    if (last_handlers == NULL) {
        last_handlers = handlers;
    } else {
        if (last_handlers == handlers) {
            printf("SUCCESS: Handler arrays merged (%p == %p)\n", last_handlers, handlers);
        } else {
            printf("FAILURE: Handler arrays NOT merged (%p != %p)\n", last_handlers, handlers);
        }
    }
}

int main() {
    int a = 1, b = 2;
    test_call(test_f("{a}", HANDLERS));
    test_call(test_f("{b}", HANDLERS));
    return 0;
}
// test_PASS_REGULAR_EXPRESSION SUCCESS: Handler arrays merged
