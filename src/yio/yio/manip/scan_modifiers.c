/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <limits.h>

int _yIO_scan_ignore(yio_scanctx_t *t) {
	yio_scanctx_get_fmt(t)->ignore = '*';
	return yio_scanctx_next(t);
}

int _yIO_scan_width_int(yio_scanctx_t *t) {
	yio_scanctx_get_fmt(t)->width = yio_scanctx_va_arg(t, int);
	return yio_scanctx_next(t);
}

int _yIO_scan_width_size_t(yio_scanctx_t *t) {
	const size_t width = yio_scanctx_va_arg(t, size_t);
	if (width > INT_MAX) {
		return YIO_ERROR_FMT_INVALID;
	}
	yio_scanctx_get_fmt(t)->width = width;
	return yio_scanctx_next(t);
}
