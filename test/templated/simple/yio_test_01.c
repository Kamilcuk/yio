#include <yπio.h>
int main() {
	int arg = 1;
	yπprintf(Yc("{}\n"), arg);
	long larg = 2;
	yπprintf(Yc("{}\n"), larg);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 2
