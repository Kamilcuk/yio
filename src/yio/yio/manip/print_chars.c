/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"

int YYΩIO_print_char(yπio_printctx_t *t) {
	const int arg = yπio_printctx_va_arg_promote(t, char);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const char buf = (char)arg;
	switch (pf->type) {
	case TC('\0'):
	case TC('c'):
		return yπio_printctx_put(t, &buf, 1);
	case TC('b'):
	case TC('B'):
	case TC('d'):
	case TC('o'):
	case TC('x'):
	case TC('X'):
		return YYΩIO_print_uschar_in(t, arg, false);
	}
	return YIO_ERROR_INVALID_TYPE;
}

int YYΩIO_print_constcharpnt(yπio_printctx_t *t) {
	const char *const str1 = yπio_printctx_va_arg(t, const char *);
	const char *const str = str1 == NULL ? "(null)" : str1;
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	if (pf->type != TC('\0') && pf->type != TC('s')) {
		return YIO_ERROR_INVALID_TYPE;
	}
	const size_t len = pf->precision >= 0 ? YYIO_strnlen(str, pf->precision) : strlen(str);
	return yπio_printctx_put(t, str, len);
}

