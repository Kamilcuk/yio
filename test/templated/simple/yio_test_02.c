#include <yio.h>
int main() {
	int arg = 1;
	yprint(arg);
	long larg = -2;
	size_t hello = 44;
	const char str[] = "Hello world";
	yprint(" ", arg, " ", larg, " ", hello, " ", str, "\n");
}
// PASS_REGULAR_EXPRESSION 1 -2 44 Hello world
