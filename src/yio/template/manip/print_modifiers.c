/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <stddef.h>

int _yΩIO_print_right(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->align = '>';
	return yπio_printctx_next(t);
}

int _yΩIO_print_left(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->align = '<';
	return yπio_printctx_next(t);
}

int _yΩIO_print_center(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->align = '^';
	return yπio_printctx_next(t);
}

int _yΩIO_print_fixed(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->type = 'f';
	return yπio_printctx_next(t);
}

int _yΩIO_print_scientific(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->type = 'e';
	return yπio_printctx_next(t);
}

int _yΩIO_print_hexfloat(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->type = 'a';
	return yπio_printctx_next(t);
}

int _yΩIO_print_precision(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->precision = yπio_printctx_va_arg(t, int);
	return yπio_printctx_next(t);
}

int _yΩIO_print_width_size_t(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->width = yπio_printctx_va_arg(t, size_t);
	return yπio_printctx_next(t);
}

int _yΩIO_print_width(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->width = yπio_printctx_va_arg(t, int);
	return yπio_printctx_next(t);
}

int _yΩIO_print_unitbuf(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->hash = '#';
	return yπio_printctx_next(t);
}

int _yΩIO_print_nounitbuf(yπio_printctx_t *t) {
	yπio_printctx_get_fmt(t)->hash = 0;
	return yπio_printctx_next(t);
}

int _yΩIO_print_pfmt(yπio_printctx_t *t) {
	const Ychar *str = yπio_printctx_va_arg(t, const Ychar *);
	const Ychar *endptr = NULL;
	if (str[0] == Yc('{')) {
		str++;
	}
	if (str[0] != Yc('}') && str[0] != Yc(':')) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	if (str[0] == Yc(':')) {
		str++;
		int err = _yΩIO_pfmt_parse(t, &t->pf, str, &endptr);
		if (err) return err;
	}
	if ((size_t)(endptr - str) != Ystrlen(str)) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yπio_printctx_next(t);
}


