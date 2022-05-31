#include <yio_test.h>
#include <yπio.h>
int main() {
	float arg = 123.456789;
	int err = yπprintf(TC("{}\n"), arg);
	YYIO_TEST(err > 6, "err=%d (%s)", err, yio_strerror(err));
	err = yπprintf(TC("{}\n"), arg);
	YYIO_TEST(err > 6, "err=%d (%s)", err, yio_strerror(err));
}
// PASS_REGULAR_EXPRESSION 123.45[67][0-9]*
// PASS_REGULAR_EXPRESSION 123.45[67][0-9]*
