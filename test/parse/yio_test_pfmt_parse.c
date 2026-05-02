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

static struct yio_printfmt_s zero = {0};

static inline
void test_pfmt(struct yio_printfmt_s shouldbe, const char *str) {
	struct yio_printfmt_s pf = {0};
	const char *endptr = (const char *)-1;
	const int err = YYIO_pfmt_parse((void*)1, &pf, str, &endptr);
	YIO_TESTEXPR(err == 0, "%s", str ? str : "(NULL)");
	YIO_TESTEXPR(endptr == str + strlen(str), "%s", str ? str : "(NULL)");
	(void)YYIO_test_compare_printformat(pf, shouldbe);
}


int main() {
	struct yio_printfmt_s pf;
	pf = zero; test_pfmt(pf, "}");
	pf = zero; pf.align = '<'; test_pfmt(pf, "<}");
	pf = zero; pf.grouping = '_'; test_pfmt(pf, "_}");
	pf = zero; pf.grouping = ','; test_pfmt(pf, ",}");
	pf = zero; pf.align = '<'; pf.fill = '%'; test_pfmt(pf, "%<}");
	pf = zero; pf.align = '>'; pf.fill = '%'; test_pfmt(pf, "%>}");
	pf = zero; pf.align = '='; pf.fill = '%'; test_pfmt(pf, "%=}");
	pf = zero; pf.align = '^'; pf.fill = '%'; test_pfmt(pf, "%^}");
	return 0;
}
