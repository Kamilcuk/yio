/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <limits.h>
#include <stdio.h>

static inline
int _yΩIO_print_wcharpnt_in(yπio_printctx_t *t, const wchar_t *ws) {
	int ret = yπio_printctx_init(t);
	if (ret) return ret;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const size_t ws_len = pf->precision >= 0 ? _yIO_wstrnlen(ws, pf->precision) : wcslen(ws);
	const Ychar *dst;
	size_t dst_len;
	ret = _yIO_strconv_wstr_to_πstr(ws, ws_len, &dst, &dst_len);
	if (ret) return ret;
	ret = yπio_printctx_putπ(t, dst, dst_len);
	_yIO_strconv_free_wstr_to_πstr(ws, dst);
	return ret;
}

int _yΩIO_print_wchar(yπio_printctx_t *t) {
	const wchar_t wc[] = { yπio_printctx_va_arg(t, wchar_t), L'\0' };
	return _yΩIO_print_wcharpnt_in(t, wc);
}

int _yΩIO_print_constwcharpnt(yπio_printctx_t *t) {
	// TODO: precision
	return _yΩIO_print_wcharpnt_in(t, yπio_printctx_va_arg(t, const wchar_t *));
}

