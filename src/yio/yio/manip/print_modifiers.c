/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config(yio);
#include "private.h"
#include <stddef.h>

int _yIO_print_right(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->align = '>';
	return yio_printctx_next(t);
}

int _yIO_print_left(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->align = '<';
	return yio_printctx_next(t);
}

int _yIO_print_center(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->align = '^';
	return yio_printctx_next(t);
}

int _yIO_print_fixed(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'f';
	return yio_printctx_next(t);
}

int _yIO_print_scientific(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'e';
	return yio_printctx_next(t);
}

int _yIO_print_hexfloat(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->type = 'a';
	return yio_printctx_next(t);
}

int _yIO_print_precision(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->precision = yio_printctx_va_arg(t, int);
	return yio_printctx_next(t);
}

int _yIO_print_width_size_t(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->width = yio_printctx_va_arg(t, size_t);
	return yio_printctx_next(t);
}

int _yIO_print_width(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->width = yio_printctx_va_arg(t, int);
	return yio_printctx_next(t);
}

int _yIO_print_unitbuf(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->hash = '#';
	return yio_printctx_next(t);
}

int _yIO_print_nounitbuf(yio_printctx_t *t) {
	yio_printctx_get_fmt(t)->hash = 0;
	return yio_printctx_next(t);
}

int _yIO_print_cfmt(yio_printctx_t *t) {
	const char *str = yio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	int err = _yIO_cfmt_parse(t, &t->pf, str, &endptr);
	if (err) return err;
	if ((size_t)(endptr - str) != Ystrlen(str)) {
		return YIO_ERROR_CFMT_INVALID;
	}
	return yio_printctx_next(t);
}

int _yIO_print_pfmt(yio_printctx_t *t) {
	const char *str = yio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	int err = _yIO_pfmt_parse(t, &t->pf, str, &endptr);
	if (err) return err;
	if ((size_t)(endptr - str) != Ystrlen(str)) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yio_printctx_next(t);
}


