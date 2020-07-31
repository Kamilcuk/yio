#include <yπio.h>
int main() {
	int arg = 1;
	yπprint(arg);
	long larg = -2;
	size_t hello = 44;
	const Ychar str[] = Yc("Hello world");
	yπprint(Yc(" "), arg, Yc(" "), larg, Yc(" "), hello, Yc(" "), str, Yc("\n"));
}
// PASS_REGULAR_EXPRESSION 1 -2 44 Hello world
