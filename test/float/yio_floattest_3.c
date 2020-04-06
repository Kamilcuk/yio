#include <yio.h>
int main() {
	long double a = 123.456789;
	yprint(a);
}
// PASS_REGULAR_EXPRESSION 123.456789[0-9]*
