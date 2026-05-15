#include "conftest.h"

int libtest_main(void) {
    yio_print("Exiting with 123\n");
    return 123;
}
// EXPECTED_EXIT_CODE 123
// PASS_REGULAR_EXPRESSION Exiting with 123
