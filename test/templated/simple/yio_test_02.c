#include <yπio.h>
int main() {
	int arg = 1;
	yπprint(arg);
	long larg = -2;
	size_t hello = 44;
	const TCHAR str[] = TC("Hello world");
	yπprint(TC(" "), arg, TC(" "), larg, TC(" "), hello, TC(" "), str, TC("\n"));
}
// PASS_REGULAR_EXPRESSION 1 -2 44 Hello world
