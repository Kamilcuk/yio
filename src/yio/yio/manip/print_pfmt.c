/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <stddef.h>

int YYΩIO_print_pfmt(yπio_printctx_t *t) {
	const TCHAR *str = yπio_printctx_va_arg(t, const TCHAR *);
	const TCHAR *endptr = NULL;
	if (str[0] == TC('{')) {
		str++;
	}
	if (str[0] != TC('}') && str[0] != TC(':')) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	if (str[0] == TC(':')) {
		str++;
		int err = YYΩIO_pfmt_parse(t, &t->pf, str, &endptr);
		if (err) return err;
	}
	if ((size_t)(endptr - str) != TSTRLEN(str)) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yπio_printctx_next(t);
}


