/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio() m4_dnl;
#include "private.h"
#include <limits.h>
#include <stdio.h>

static inline
int yio_printctx_out_wchar(yio_printctx_t *t, wchar_t wc) {
	char s[MB_LEN_MAX];
	const int len = wctomb(s, wc);
	if (len < 0) return YIO_ERROR_WCTOMB_ERR;
	return yio_printctx_raw_write(t, s, len);
}

int _yIO_print_wchar(yio_printctx_t *t) {
	wchar_t wc = yio_printctx_va_arg(t, wchar_t);
	return yio_printctx_out_wchar(t, wc);
}

static inline
int _yIO_print_wcharpnt_in(yio_printctx_t *t, const wchar_t *ws) {
	for (; *ws != L'\0'; ws++) {
		const int err = yio_printctx_out_wchar(t, *ws);
		if (err) return err;
	}
	return 0;
}

int _yIO_print_wcharpnt(yio_printctx_t *t) {
	return _yIO_print_wcharpnt_in(t, yio_printctx_va_arg(t, wchar_t *));
}

int _yIO_print_constwcharpnt(yio_printctx_t *t) {
	return _yIO_print_wcharpnt_in(t, yio_printctx_va_arg(t, const wchar_t *));
}
