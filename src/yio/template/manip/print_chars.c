/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"

static inline
int _yΩIO_print_constornot_charpnt(yπio_printctx_t *t, const char *str) {
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	if (pf->type != TC('\0') && pf->type != TC('s')) {
		return YIO_ERROR_INVALID_TYPE;
	}
	const size_t len = pf->precision >= 0 ? _yIO_strnlen(str, pf->precision) : strlen(str);
	return yπio_printctx_put(t, str, len);
}

static inline
int _yΩIO_print_generic_char(yπio_printctx_t *t, int arg) {
	const char buf[2] = { arg, 0 };
	return _yΩIO_print_constornot_charpnt(t, buf);
}

/* ------------------------------------------------------ */

int _yΩIO_print_char(yπio_printctx_t *t) {
	const int arg = yπio_printctx_va_arg_num(t, char);
	return _yΩIO_print_generic_char(t, arg);
}

int _yΩIO_print_schar(yπio_printctx_t *t) {
	const int arg = yπio_printctx_va_arg_num(t, signed char);
	return _yΩIO_print_generic_char(t, arg);
}

int _yΩIO_print_uchar(yπio_printctx_t *t) {
	const int arg = yπio_printctx_va_arg_num(t, unsigned char);
	return _yΩIO_print_generic_char(t, arg);
}

int _yΩIO_print_constcharpnt(yπio_printctx_t *t) {
	const char * const arg = yπio_printctx_va_arg(t, const char *);
	return _yΩIO_print_constornot_charpnt(t, arg);
}

