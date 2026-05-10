#include "conftest.h"

int libtest_main(void) {
	YIO_TEST_EQ("123", "{}", 123);
	YIO_TEST_EQ("0123", "{:04}", 123);
	YIO_TEST_EQ(" 123", "{:4}", 123);
	YIO_TEST_EQ("123 ", "{:<4}", 123);
	YIO_TEST_EQ("+123", "{:+}", 123);
	return 0;
}
