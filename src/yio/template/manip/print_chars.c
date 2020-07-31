/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config_yio_template(`m4_dnl);
#include "private.h"

static inline
int _yΩIO_print_constornot_charpnt(yπio_printctx_t *t, const char *arg) {
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return yio_printctx_puts(t, arg);
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

int _yΩIO_print_charpnt(yπio_printctx_t *t) {
	const char * const arg = yπio_printctx_va_arg(t, char *);
	return _yΩIO_print_constornot_charpnt(t, arg);
}

int _yΩIO_print_constcharpnt(yπio_printctx_t *t) {
	const char * const arg = yπio_printctx_va_arg(t, const char *);
	return _yΩIO_print_constornot_charpnt(t, arg);
}

~)m4_dnl;
