#include <yio_test.h>
#include <complex.h>
int main() {
	_Complex float a = 1 - 1 * I;
	YIO_TEST("1-1i", "{}", a);
	_Complex double b = 1 + 1 * I;
	YIO_TEST("1+1i", "{}", b);
}

