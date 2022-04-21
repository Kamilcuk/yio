#include "yio_test.h"
#include <string.h>

#define YIO_TEST(shouldbe, fmt, ...)  \
	do { \
		char buf[100]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		_yIO_TEST(err > 0, "fmt=`%s` err=%s", fmt, yio_strerror(err)); \
		_yIO_TEST(strcmp(shouldbe, buf) == 0, "shouldbe=`%s` buf=`%s`", shouldbe, buf); \
	} while(0)

#define YIO_TEST_FAIL(fmt, ...)  \
	do { \
		char buf[100]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		_yIO_TEST(err < 0, "fmt=`%s` err=%s", fmt, yio_strerror(err)); \
	} while(0)

int main() {
	YIO_TEST(" B", " B");
	YIO_TEST(" { } } { { ", " {{ }} }} {{ {{ ");
	YIO_TEST(" 1", " {}", "1");
	YIO_TEST(" A", " {}", "A");
	YIO_TEST(" Hello World! ", " {:}{: 6}{} ", "Hello", "World", (char)'!');
	YIO_TEST("}", "}}");
	YIO_TEST("{", "{{");
	YIO_TEST("{}", "{{}}");
	YIO_TEST("}{", "}}{{");
	YIO_TEST("a}b{c", "a}}b{{c");
	YIO_TEST("}b{", "}}b{{");
	YIO_TEST("a}{b", "a}}{{b");

	YIO_TEST_FAIL("{");
	YIO_TEST_FAIL("}");
	YIO_TEST_FAIL("}}}");
	YIO_TEST_FAIL("{{{");
	YIO_TEST_FAIL("{{}");
	YIO_TEST_FAIL("}}{");
	YIO_TEST_FAIL("}}{  {{");
}
