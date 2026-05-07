#include <yio.h>
#include <yio_test.h>

#ifndef YYIO_HAS_STDFIX_TYPES
#error YYIO_HAS_STDFIX_TYPES is not defined
#endif

#ifdef fract
#error leaked stdfix.h to public headers
#endif

int main(void) {
#if YYIO_HAS_STDFIX_TYPES
	_Fract f1 = 0.5r;
	_Accum a1 = 1.5k;
	_Fract f2 = 0.25r;
	_Accum a2 = 2.75k;

	/* Static positional index tests */
	YIO_TEST("0.500000 1.500000", "{0} {1}", f1, a1);
	YIO_TEST("1.500000 0.500000", "{1} {0}", f1, a1);
	YIO_TEST("0.250000 0.500000 2.750000", "{2} {0} {3}", f1, a1, f2, a2);
	YIO_TEST("2.750000 1.500000 1.500000", "{3} {1} {1}", f1, a1, f2, a2);

	/* Dynamic precision using positional indices */
	YIO_TEST("0.5 1.5", "{0:.{2}} {1:.{2}}", f1, a1, 1);
	YIO_TEST("0.50 1.500", "{2:.{0}} {3:.{1}}", 2, 3, f1, a1);
	
	/* Width and precision from other arguments */
	YIO_TEST("  0.50", "{2:{0}.{1}}", 6, 2, f1);
	YIO_TEST(" 1.5", "{2:{0}.{1}}", 4, 1, a1);
	
	/* Complex reordering with stdfix types */
	YIO_TEST("2.75 0.500 1.50 0.3", "{4:.2} {1:.{5}} {2:.{6}} {3:.{0}}", 1, f1, a1, f2, a2, 3, 2);

	return 0;
#else
	return 77;
#endif
}
