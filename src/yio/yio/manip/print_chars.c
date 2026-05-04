/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <string.h>

static size_t YYIO_strnlen(const char *str, size_t maxlen) {
#if YYIO_HAS_strnlen && !defined(__SDCC)
	return strnlen(str, maxlen);
#else
	const char *str0 = str;
	while (maxlen-- > 0 && *str) {
		++str;
	}
	return str - str0;
#endif
}

int YYIO_print_char(yio_printctx_t *t) {
#if UINT_MAX >= UCHAR_MAX
	typedef int promoted_char;
#else
	typedef unsigned promoted_char;
#endif
  const char arg = yio_printctx_va_arg(t, promoted_char);
	const int err = yio_printctx_init_or_number(t, arg);
	if (err) return err;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	switch (pf->type) {
		case '\0':
		case 'c':
			return yio_printctx_put(t, &arg, 1);
		case 'b':
		case 'B':
		case 'd':
		case 'o':
		case 'x':
		case 'X':
			return YYIO_print_uint_in(t, arg, false);
		default:
			return YIO_ERROR_INVALID_TYPE;
	}
	return YIO_ERROR_INVALID_TYPE;
}

int YYIO_print_constcharpnt(yio_printctx_t *t) {
	const char *const str1 = yio_printctx_va_arg(t, const char *);
	const int err = yio_printctx_init(t);
	if (err) return err;
	const char *const str = str1 == NULL ? "(null)" : str1;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	if (pf->type != '\0' && pf->type != 's') {
		return YIO_ERROR_INVALID_TYPE;
	}
	const size_t precision = yio_precision_get_default(pf->precision, SIZE_MAX);
	const size_t len = precision != SIZE_MAX ? YYIO_strnlen(str, precision) : strlen(str);
	return yio_printctx_put(t, str, len);
}

