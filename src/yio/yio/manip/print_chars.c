/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"

static inline
int YYΩIO_print_constornot_charpnt(yπio_printctx_t *t, const char *str) {
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	if (pf->type != TC('\0') && pf->type != TC('s')) {
		return YIO_ERROR_INVALID_TYPE;
	}
	const size_t len = pf->precision >= 0 ? YYIO_strnlen(str, pf->precision) : strlen(str);
	return yπio_printctx_put(t, str, len);
}

/* ------------------------------------------------------ */

int YYΩIO_print_char(yπio_printctx_t *t) {
	const int arg = yπio_printctx_va_arg_promote(t, char);
	const char buf[2] = { (char)arg, 0 };
	return YYΩIO_print_constornot_charpnt(t, buf);
}

int YYΩIO_print_constcharpnt(yπio_printctx_t *t) {
	const char *const arg = yπio_printctx_va_arg(t, const char *);
	return YYΩIO_print_constornot_charpnt(t, arg != NULL ? arg : "(null)");
}

