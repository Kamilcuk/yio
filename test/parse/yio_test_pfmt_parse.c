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
	struct yio_printfmt_s pf = YYIO_printfmt_default;
	const char *endptr = (const char *)-1;
	const int err = YYIO_pfmt_parse((void*)1, &pf, str, &endptr);
	YYIO_TEST(err == 0, "%s", str ? str : "(NULL)");
	YYIO_TEST(endptr == str + strlen(str), "%s", str ? str : "(NULL)");
	(void)YYIO_test_compare_printformat(pf, shouldbe);
}


int main() {
	struct yio_printfmt_s pf;
	pf = YYIO_printfmt_default; test_pfmt(pf, "}");
	pf = YYIO_printfmt_default; pf.align = '<'; test_pfmt(pf, "<}");
	pf = YYIO_printfmt_default; pf.grouping = '_'; test_pfmt(pf, "_}");
	pf = YYIO_printfmt_default; pf.grouping = ','; test_pfmt(pf, ",}");
	pf = YYIO_printfmt_default; pf.align = '<'; pf.fill = '%'; test_pfmt(pf, "%<}");
	pf = YYIO_printfmt_default; pf.align = '>'; pf.fill = '%'; test_pfmt(pf, "%>}");
	pf = YYIO_printfmt_default; pf.align = '='; pf.fill = '%'; test_pfmt(pf, "%=}");
	pf = YYIO_printfmt_default; pf.align = '^'; pf.fill = '%'; test_pfmt(pf, "%^}");
	return 0;
}
