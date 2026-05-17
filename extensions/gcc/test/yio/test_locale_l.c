#include <yio.h>
#include <assert.h>
#include "conftest.h"

//// RUN
//// RUN PASS_REGULAR_EXPRESSION "Val: 1234"

int main() {
    int val = 1234;
    // The 'L' should be parsed and reconstructed correctly.
    // If optimized, it should still result in "1234" (assuming default locale for tests).
    int err = yio_print_f("Val: {val:Ld}\n");
    assert(err > 0);
    return 0;
}
