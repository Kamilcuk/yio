/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <stddef.h>

int YIO_print_pfmt(yio_printctx_t *t) {
	const char *str = yio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	if (str[0] == '{') {
		str++;
	}
	if (str[0] != '}' && str[0] != ':') {
		return YIO_ERROR_PYFMT_INVALID;
	}
	if (str[0] == ':') {
		str++;
		int err = YIO_pfmt_parse(t, &t->pf, str, &endptr);
		if (err) return err;
	}
	if ((size_t)(endptr - str) != strlen(str)) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yio_printctx_next(t);
}


