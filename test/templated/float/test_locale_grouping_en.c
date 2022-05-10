/*
 * test_grouping.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio_test.h>
#ifndef YIO_USE_LOCALE
#error
#endif
#if !YIO_USE_LOCALE
int main() { return 100; }
#else

#include <locale.h>
#include <assert.h>
#include <langinfo.h>

int main() {
	const char *const loc = setlocale(LC_ALL, "en_US.UTF-8");
	if (loc == NULL) {
		return 100;
		// SKIP_RETURN_CODE 100
	}
	/*printf("%'030ld\n", 12345354636534);*/
	YΩIO_TEST("1.125000", "{:Lf}", 1.125);
	YΩIO_TEST("21.125000", "{:Lf}", 21.125);
	YΩIO_TEST("321.125000", "{:Lf}", 321.125);
#ifdef __GLIBC__
	YΩIO_TEST("4,321.125000", "{:Lf}", 4321.125);
	YΩIO_TEST("54,321.125000", "{:Lf}", 54321.125);
	YΩIO_TEST("654,321.125000", "{:Lf}", 654321.125);
	YΩIO_TEST("7,654,321.125000", "{:Lf}", 7654321.125);
	YΩIO_TEST("87,654,321.125000", "{:Lf}", 87654321.125);
	YΩIO_TEST("987,654,321.125000", "{:Lf}", 987654321.125);
	YΩIO_TEST("1,987,654,321.125000", "{:Lf}", 1987654321.125);
	YΩIO_TEST("21,987,654,321.125000", "{:Lf}", 21987654321.125);
	YΩIO_TEST("321,987,654,321.125000", "{:Lf}", 321987654321.125);
	YΩIO_TEST("4,321,987,654,321.125000", "{:Lf}", 4321987654321.125);
	YΩIO_TEST("54,321,987,654,321.125000", "{:Lf}", 54321987654321.125);
	YΩIO_TEST("654,321,987,654,321.125000", "{:Lf}", 654321987654321.125);
	YΩIO_TEST("7,654,321,987,654,321.000000", "{:Lf}", 7654321987654321.000);
#endif
	return 0;

}

#endif
