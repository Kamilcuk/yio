#include <yπio.h>
int main() {
	double a = 123.456789;
	yπprint(a, "\n");
	yπprint(a, "\n");
}
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
