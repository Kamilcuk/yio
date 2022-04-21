#include <yio_test.h>

static const struct tm tm = {0};
int main() {
	YIO_TEST("5", "{}", 5);
	YIO_TEST("5", "{:d}", 5);
	YIO_TEST("5", "{:x}", 5);
	YIO_TEST("101", "{:b}", 5);
	YIO_TEST_FAIL("{:f}", 5);

	YIO_TEST_FAIL("{", tm);
	YIO_TEST_FAIL("{aaaa", tm);
	YIO_TEST(" A", "{:2A}", tm);
	YIO_TEST(" A ", "{:^3A}", tm);
	YIO_TEST("BAB", "{:B^3A}", tm);
}

