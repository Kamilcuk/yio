#include <yio_test.h>
#include <math.h>
int main() {
#if defined(INFINITY) && defined(NAN)
	YIO_TEST("  inf", "{:05}", INFINITY);
	YIO_TEST("00inf", "{:0>5}", INFINITY);
#endif


