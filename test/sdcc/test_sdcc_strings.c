#include "conftest.h"

int libtest_main(void) {
	char *p = (char *)"abc";
	const char *cp = "def";
	char arr[] = "ghi";
	const char carr[] = "jkl";

	YIO_TEST_EQ("abc", "{}", p);
	YIO_TEST_EQ("def", "{}", cp);
	YIO_TEST_EQ("ghi", "{}", &arr[0]);
	YIO_TEST_EQ("jkl", "{}", &carr[0]);
	YIO_TEST_EQ("xyz", "{}", "xyz");

	return 0;
}
