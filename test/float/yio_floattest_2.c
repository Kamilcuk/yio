#include <yio.h>
int main() {
	double a = 123.456789;
	yprint(a);
}
// PASS_REGULAR_EXPRESSION 123.45[0-9]*
