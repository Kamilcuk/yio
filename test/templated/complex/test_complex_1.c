#include <yio_test.h>
#include <complex.h>
int main() {
	_Complex float a = 1 - 1 * I;
	yπprintf(Yc("{}\n"), a);
	_Complex double b = 1 + 1 * I;
	yπprintf(Yc("{}\n"), b);
}
// PASS_REGULAR_EXPRESSION 1-1i
// PASS_REGULAR_EXPRESSION 1\+1i

