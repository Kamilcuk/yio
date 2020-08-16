/*
 * yio_test_pfmt_parse.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio_test_private.h>
#include <string.h>
#include <ctype.h>

// The magic (void*)1 casts are there so that
// __attribute__((__nonnull__)) doesn't throw an error.

static inline
void test_cfmt(const struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf = _yIO_printfmt_default;
	const char *endptr = (const char *)-1;
	const int err = _yIO_cfmt_parse((void*)1, &pf, str, &endptr);
	_yIO_TEST(err == 0, "%s", str);
	_yIO_TEST(endptr == str + strlen(str), "%s", str);
	_yIO_TEST(err == 0, "%s", str);
	_yIO_TEST(endptr == str + strlen(str), "%s", str);
	(void)_yIO_test_compare_printformat(pf, shouldbe);
}

int main() {
	struct yio_printfmt_s pf;
	pf = _yIO_printfmt_default; pf.type = 'd'; test_cfmt(pf, "%d");
	pf = _yIO_printfmt_default; pf.type = 'd'; test_cfmt(pf, "%lld");
	pf = _yIO_printfmt_default; pf.align = '<'; pf.fill = '0'; pf.type = 'A';
		test_cfmt(pf, "%-0A");
	pf = _yIO_printfmt_default; pf.hash = '#'; pf.fill = '0'; pf.type = 'g';
		test_cfmt(pf, "%#0g");
	pf = _yIO_printfmt_default; pf.hash = '#'; pf.sign = '+'; pf.width = 1; pf.precision = 10;
		pf.fill = '0'; pf.type = 'f'; test_cfmt(pf, "%+#01.10f");
	pf = _yIO_printfmt_default; pf.hash = '#'; pf.sign = '+'; pf.width = 1; pf.precision = 10;
		pf.type = 'f'; test_cfmt(pf, "%+#1.10f");
	return 0;
}
