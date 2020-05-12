/*
 * yio_test_pfmt_parse.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio_test_private.h>
#include <string.h>

static inline
void test_pfmt(struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf;
	memset(&pf, -1, sizeof(pf));
	const char *endptr = (const char *)-1;
	const int err = _yIO_pfmt_parse((void*)1, &pf, str, &endptr);
	_yIO_TEST_MSG(err == 0, "%s", str);
	_yIO_TEST_MSG(endptr == str + strlen(str), "%s", str);
	_yIO_TEST_MSG(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str);
}

static inline
void test_cfmt(struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf;
	memset(&pf, -1, sizeof(pf));
	const char *endptr = (const char *)-1;
	const int err = _yIO_cfmt_parse((void*)1, &pf, str, &endptr);
	_yIO_TEST_MSG(err == 0, "%s", str);
	_yIO_TEST_MSG(endptr == str + strlen(str), "%s", str);
	_yIO_TEST_MSG(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str);
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
