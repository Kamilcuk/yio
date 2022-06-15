#include "yio_test.h"
#include <string.h>

int main() {
	YIO_TEST(" B", " B");
	YIO_TEST(" { } } { { ", " {{ }} }} {{ {{ ");
	YIO_TEST(" 1", " {}", "1");
	YIO_TEST(" A", " {}", "A");
	YIO_TEST(" Hello World! ", " {:}{: >6}{} ", "Hello", "World", (char)'!');
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
