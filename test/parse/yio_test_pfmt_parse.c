/*
 * yio_test_pfmt_parse.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio_test_private.h>
#include <string.h>

// The magic (void*)1 casts are there so that
// __attribute__((__nonnull__)) doesn't throw an error.

static inline
void test_pfmt(struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf = _yIO_printfmt_default;
	const char *endptr = (const char *)-1;
	const int err = _yIO_pfmt_parse((void*)1, &pf, str, &endptr);
	_yIO_TEST(err == 0, "%s", str);
	_yIO_TEST(endptr == str + strlen(str), "%s", str);
	_yIO_TEST(pf.width == shouldbe.width);
	_yIO_TEST(pf.precision == shouldbe.precision);
	_yIO_TEST(pf.fill == shouldbe.fill, "%s", str);
	_yIO_TEST(pf.align == shouldbe.align, "%s", str);
	_yIO_TEST(pf.sign == shouldbe.sign, "%s", str);
	_yIO_TEST(pf.grouping == shouldbe.grouping);
	_yIO_TEST(pf.type == shouldbe.type);
	_yIO_TEST(pf.hash == shouldbe.hash);
	_yIO_TEST(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str);
}

static inline
void test_cfmt(struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf;
	memset(&pf, -1, sizeof(pf));
	const char *endptr = (const char *)-1;
	const int err = _yIO_cfmt_parse((void*)1, &pf, str, &endptr);
	_yIO_TEST(err == 0, "%s", str);
	_yIO_TEST(endptr == str + strlen(str), "%s", str);
	_yIO_TEST(err == 0, "%s", str);
	_yIO_TEST(endptr == str + strlen(str), "%s", str);
	_yIO_TEST(pf.width == shouldbe.width);
	_yIO_TEST(pf.precision == shouldbe.precision);
	_yIO_TEST(pf.fill == shouldbe.fill);
	_yIO_TEST(pf.align == shouldbe.align);
	_yIO_TEST(pf.sign == shouldbe.sign);
	_yIO_TEST(pf.grouping == shouldbe.grouping);
	_yIO_TEST(pf.type == shouldbe.type);
	_yIO_TEST(pf.hash == shouldbe.hash);
	_yIO_TEST(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str);
}

int main() {
	struct yio_printfmt_s pf;
	pf = _yIO_printfmt_default; test_pfmt(pf, "{}");
	pf = _yIO_printfmt_default; test_pfmt(pf, "{:}");
	pf = _yIO_printfmt_default; pf.align = '<'; test_pfmt(pf, "{:<}");
	pf = _yIO_printfmt_default; pf.grouping = '_'; test_pfmt(pf, "{:_}");
	pf = _yIO_printfmt_default; pf.grouping = ','; test_pfmt(pf, "{:,}");
	pf = _yIO_printfmt_default; pf.align = '<'; pf.fill = '%'; test_pfmt(pf, "{:%<}");
	pf = _yIO_printfmt_default; pf.align = '>'; pf.fill = '%'; test_pfmt(pf, "{:%>}");
	pf = _yIO_printfmt_default; pf.align = '='; pf.fill = '%'; test_pfmt(pf, "{:%=}");
	pf = _yIO_printfmt_default; pf.align = '^'; pf.fill = '%'; test_pfmt(pf, "{:%^}");
	return 0;
}
