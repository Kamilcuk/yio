#include <yπio.h>
int main() {
	long double a = 123.456789;
	yπprint(a);
}
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
