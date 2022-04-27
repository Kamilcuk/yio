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
	YIO_TEST_FAIL("{:aaaa", tm);
	YIO_TEST(" A", "{:2A}", tm);
	YIO_TEST(" A ", "{:^3A}", tm);
	YIO_TEST("BAB", "{:B^3A}", tm);
	YIO_TEST("BAB", "{:B^3.3A}", tm);

	int count;
	int count2 = yprintf("Hello\n{}", ypcount(&count));
	_yIO_TEST(count2 == 6);
	_yIO_TEST(count == 6);

	//YIO_TEST("\\x02", "{!a}", "\x02");

	const void *p = &tm;
	yprintf("{}", p);
	yprintf("{:p}", p);
}

