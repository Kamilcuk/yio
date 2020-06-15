/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"

static inline
int _yIO_print_generic_char(yio_printctx_t *t, int arg) {
	const char buf[2] = { arg, 0 };
	return _yIO_printformat_string(t, buf);
}

int _yIO_print_char(yio_printctx_t *t) {
	const int arg = yio_printctx_va_arg_num(t, char);
	return _yIO_print_generic_char(t, arg);
}

int _yIO_print_schar(yio_printctx_t *t) {
	const int arg = yio_printctx_va_arg_num(t, signed char);
	return _yIO_print_generic_char(t, arg);
}

int _yIO_print_uchar(yio_printctx_t *t) {
	const int arg = yio_printctx_va_arg_num(t, unsigned char);
	return _yIO_print_generic_char(t, arg);
}

static inline
int _yIO_print_constornot_charpnt(yio_printctx_t *t, const char *arg) {
	return _yIO_printformat_string(t, arg);
}

int _yIO_print_charpnt(yio_printctx_t *t) {
	const char * const arg = yio_printctx_va_arg(t, char *);
	return _yIO_print_constornot_charpnt(t, arg);
}

int _yIO_print_constcharpnt(yio_printctx_t *t) {
	const char * const arg = yio_printctx_va_arg(t, const char *);
	return _yIO_print_constornot_charpnt(t, arg);
}
