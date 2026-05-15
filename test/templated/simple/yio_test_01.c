#include <yio.h>
int main() {
	int arg = 1;
	yio_print("{}\n", arg);
	long larg = 2;
	yio_print("{}\n", larg);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 2
