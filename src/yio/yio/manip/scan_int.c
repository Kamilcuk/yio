/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <ctype.h>

static inline
int _yIO_scan_ignore_whitespaces(yio_scanctx_t *t) {
	int err = 0;
	for (int c; (err = yio_scanctx_in(t, &c)) == 0 && c != EOF;) {
		// _yIO_dbgln("c=%x", c);
		if (c != ' ' && c != '\t' && c != '\n') {
			break;
		}
	}
	yio_scanctx_unin(t);
	return err;
}

int _yIO_scan_int(yio_scanctx_t *t) {
	int * const dest = yio_scanctx_va_arg(t, int *);
	int err = 0;
	int res = 0;

	err = _yIO_scan_ignore_whitespaces(t);
	if (err) {
		goto EXIT;
	}

	char minus = 0;
	for (int c; (err = yio_scanctx_in(t, &c)) == 0 && c != EOF;) {
		if (minus == 0 && (c == '-' || c == '+')) {
			minus = c;
			continue;
		}
		if (!isdigit((unsigned char)c)) {
			break;
		}
		const char n = c - '0';
		if (minus != '-') {
			if (res > INT_MAX / 10 - n) {
				err = YIO_ERROR_SCAN_OVERFLOW;
				break;
			}
			res *= 10;
			res += n;
		} else {
			if (res < INT_MIN / 10 + n) {
				err = YIO_ERROR_SCAN_UNDERFLOW;
				break;
			}
			res *= 10;
			res -= n;
		}
	}
	yio_scanctx_unin(t);

	EXIT:
	if (!yio_scanctx_get_fmt(t)->ignore) {
		*dest = res;
	}
	return err;
}
