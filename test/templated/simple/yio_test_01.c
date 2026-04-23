#include <yio.h>
int main() {
	int arg = 1;
	yprintf("{}\n", arg);
	long larg = 2;
	yprintf("{}\n", larg);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 2
