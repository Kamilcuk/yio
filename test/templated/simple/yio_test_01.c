#include <yio.h>
int main() {
	int arg = 1;
	yio_printf("{}\n", arg);
	long larg = 2;
	yio_printf("{}\n", larg);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 2
