/*
 * test_grouping.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
#ifndef YIO_USE_LOCALE
#error
#endif
#if !YIO_USE_LOCALE
int main() {}
#else

#include <locale.h>
#include <assert.h>

int main() {
	const char *const loc = setlocale(LC_ALL, "en_US.UTF-8");
	if (loc == NULL) {
		return 100;
		// SKIP_RETURN_CODE 100
	}
	/*printf("%'030ld\n", 12345354636534);*/
	int err = 0;

	err |= yπprintf(TC("{:Lf}\n"), 1.125);
	err |= yπprintf(TC("{:Lf}\n"), 21.125);
	err |= yπprintf(TC("{:Lf}\n"), 321.125);
	err |= yπprintf(TC("{:Lf}\n"), 4321.125);
	err |= yπprintf(TC("{:Lf}\n"), 54321.125);
	err |= yπprintf(TC("{:Lf}\n"), 654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 7654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 87654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 1987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 21987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 321987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 4321987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 54321987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 654321987654321.125);
	err |= yπprintf(TC("{:Lf}\n"), 7654321987654321.000);
	return !(err > 0);

// PASS_REGULAR_EXPRESSION 1.125000
// PASS_REGULAR_EXPRESSION 21.125000
// PASS_REGULAR_EXPRESSION 321.125000
// PASS_REGULAR_EXPRESSION 4,321.125000
// PASS_REGULAR_EXPRESSION 54,321.125000
// PASS_REGULAR_EXPRESSION 654,321.125000
// PASS_REGULAR_EXPRESSION 7,654,321.125000
// PASS_REGULAR_EXPRESSION 87,654,321.125000
// PASS_REGULAR_EXPRESSION 987,654,321.125000
// PASS_REGULAR_EXPRESSION 1,987,654,321.125000
// PASS_REGULAR_EXPRESSION 21,987,654,321.125000
// PASS_REGULAR_EXPRESSION 321,987,654,321.125000
// PASS_REGULAR_EXPRESSION 4,321,987,654,321.125000
// PASS_REGULAR_EXPRESSION 54,321,987,654,321.125000
// PASS_REGULAR_EXPRESSION 654,321,987,654,321.125000
// PASS_REGULAR_EXPRESSION 7,654,321,987,654,321.000000

}

#endif
