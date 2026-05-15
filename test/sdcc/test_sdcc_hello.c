#include "conftest.h"

int libtest_main(void) {
	yio_print("Hello world\n");
	return 0;
}
// PASS_REGULAR_EXPRESSION Hello world
