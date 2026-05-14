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
	struct yio_printfmt_s pf = {0};
	const char *endptr = (const char *)-1;
	const int err = YIO_pfmt_parse((void*)1, &pf, str, &endptr);
	YIO_TESTEXPR(err == 0, "%s", str ? str : "(NULL)");
	YIO_TESTEXPR(endptr == str + strlen(str), "%s", str ? str : "(NULL)");
	(void)YIO_test_compare_printformat(pf, shouldbe);
}


int main() {
	test_pfmt(YIO_printfmt_zero, "}");
	test_pfmt((struct yio_printfmt_s){.align = '<'}, "<}");
	test_pfmt((struct yio_printfmt_s){.grouping = '_'}, "_}");
	test_pfmt((struct yio_printfmt_s){.grouping = ','}, ",}");
	test_pfmt((struct yio_printfmt_s){.align = '<', .fill = '%'}, "%<}");
	test_pfmt((struct yio_printfmt_s){.align = '>', .fill = '%'}, "%>}");
	test_pfmt((struct yio_printfmt_s){.align = '=', .fill = '%'}, "%=}");
	test_pfmt((struct yio_printfmt_s){.align = '^', .fill = '%'}, "%^}");
	return 0;
}
