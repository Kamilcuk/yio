#include <ywio.h>
int main() {
	int arg = 1;
	ywprintf("{}", arg);
	long larg = 2;
	ywprintf("{}\n", larg);
}
// PASS_REGULAR_EXPRESSION 12
