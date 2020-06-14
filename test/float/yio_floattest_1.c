#include <yio_test.h>
int main() {
	float arg = 123.456789;
	int err = yprintf("{}", arg);
	_yIO_TEST(err > 6, "err=%d", err);
}
// PASS_REGULAR_EXPRESSION 123.45[67][0-9]*
