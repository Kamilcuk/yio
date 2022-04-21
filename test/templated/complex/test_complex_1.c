#include <yio_test.h>
#include <complex.h>
int main() {
	_Complex float a = 1 - 1 * I;
	yπprintf(TC("{}\n"), a);
	_Complex double b = 1 + 1 * I;
	yπprintf(TC("{}\n"), b);
}
// PASS_REGULAR_EXPRESSION 1-1i
// PASS_REGULAR_EXPRESSION 1\+1i

