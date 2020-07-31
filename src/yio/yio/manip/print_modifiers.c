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
	yio_printctx_get_fmt(t)->align = '>';
	return yio_printctx_next(t);
}

int _yΩIO_print_left(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->align = '<';
	return yio_printctx_next(t);
}

int _yΩIO_print_center(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->align = '^';
	return yio_printctx_next(t);
}

int _yΩIO_print_fixed(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'f';
	return yio_printctx_next(t);
}

int _yΩIO_print_scientific(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'e';
	return yio_printctx_next(t);
}

int _yΩIO_print_hexfloat(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'a';
	return yio_printctx_next(t);
}

int _yΩIO_print_precision(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->precision = yio_printctx_va_arg(t, int);
	return yio_printctx_next(t);
}

int _yΩIO_print_width_size_t(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->width = yio_printctx_va_arg(t, size_t);
	return yio_printctx_next(t);
}

int _yΩIO_print_width(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->width = yio_printctx_va_arg(t, int);
	return yio_printctx_next(t);
}

int _yΩIO_print_unitbuf(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->hash = '#';
	return yio_printctx_next(t);
}

int _yΩIO_print_nounitbuf(yπio_printctx_t *t) {
	yio_printctx_get_fmt(t)->hash = 0;
	return yio_printctx_next(t);
}

int _yΩIO_print_pfmt(yπio_printctx_t *t) {
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
		int err = _yΩIO_pfmt_parse(t, &t->pf, str, &endptr);
		if (err) return err;
	}
	if ((size_t)(endptr - str) != Ystrlen(str)) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yio_printctx_next(t);
}


