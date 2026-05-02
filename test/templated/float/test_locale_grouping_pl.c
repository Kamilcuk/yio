/*
 * test_grouping.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
#ifndef YIO_ENABLE_LOCALE
#error
#endif
#if !YIO_ENABLE_LOCALE || !__GLIBC__
int main() { return 100; }
#else

#include <locale.h>
#include <assert.h>

int main() {
	const char *const loc = setlocale(LC_ALL, "pl_PL.UTF-8");
	if (loc == NULL) {
		fprintf(stderr, "Skipping test: locale 'pl_PL.UTF-8' not found.\n"
				"To run this test, install the locale, e.g., on Debian/Ubuntu:\n"
				"  sudo locale-gen pl_PL.UTF-8\n"
				"  sudo update-locale\n");
		return 100;
		// SKIP_RETURN_CODE 100
	}
	int err = 0;

	err |= yio_printf("{:Lf}\n", 1.125);
	err |= yio_printf("{:Lf}\n", 21.125);
	err |= yio_printf("{:Lf}\n", 321.125);
	err |= yio_printf("{:Lf}\n", 4321.125);
	err |= yio_printf("{:Lf}\n", 54321.125);
	err |= yio_printf("{:Lf}\n", 654321.125);
	err |= yio_printf("{:Lf}\n", 7654321.125);
	err |= yio_printf("{:Lf}\n", 87654321.125);
	err |= yio_printf("{:Lf}\n", 987654321.125);
	err |= yio_printf("{:Lf}\n", 1987654321.125);
	err |= yio_printf("{:Lf}\n", 21987654321.125);
	err |= yio_printf("{:Lf}\n", 321987654321.125);
	err |= yio_printf("{:Lf}\n", 4321987654321.125);
	err |= yio_printf("{:Lf}\n", 54321987654321.125);
	err |= yio_printf("{:Lf}\n", 654321987654321.125);
	err |= yio_printf("{:Lf}\n", 7654321987654321.000);
	return !(err > 0);

	// Below this is __NOT__ a space.
	// It is NARROW NO-BREAK SPACE
	// https://www.fileformat.info/info/unicode/char/202f/index.htm
// PASS_REGULAR_EXPRESSION 1.125000
// PASS_REGULAR_EXPRESSION 21.125000
// PASS_REGULAR_EXPRESSION 321.125000
// PASS_REGULAR_EXPRESSION 4 321.125000
// PASS_REGULAR_EXPRESSION 54 321.125000
// PASS_REGULAR_EXPRESSION 654 321.125000
// PASS_REGULAR_EXPRESSION 7 654 321.125000
// PASS_REGULAR_EXPRESSION 87 654 321.125000
// PASS_REGULAR_EXPRESSION 987 654 321.125000
// PASS_REGULAR_EXPRESSION 1 987 654 321.125000
// PASS_REGULAR_EXPRESSION 21 987 654 321.125000
// PASS_REGULAR_EXPRESSION 321 987 654 321.125000
// PASS_REGULAR_EXPRESSION 4 321 987 654 321.125000
// PASS_REGULAR_EXPRESSION 54 321 987 654 321.125000
// PASS_REGULAR_EXPRESSION 654 321 987 654 321.125000
// PASS_REGULAR_EXPRESSION 7 654 321 987 654 321.000000

}

#endif
