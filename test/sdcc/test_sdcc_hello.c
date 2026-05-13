#include "conftest.h"

int libtest_main(void) {
	yio_printf("Hello world\n");
	return 0;
}
// PASS_REGULAR_EXPRESSION Hello world
