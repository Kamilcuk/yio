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
	const size_t ws_len = pf->precision >= 0 ? _yIO_wcsnlen(ws, pf->precision) : wcslen(ws);
m4_template_chooser(`m4_dnl);
	char *mb; size_t mb_len;
	ret = _yIO_conv_wcs_to_mbs(ws, ws_len, &mb, &mb_len);
	if (ret) return ret;
	ret = yπio_printctx_put(t, mb, mb_len);
	free(mb);
	return ret;
~, `m4_dnl;
	return yπio_printctx_putπ(t, ws, ws_len);
~, `m4_dnl;
	size_t length = 0;
	uint32_t *result = u32_conv_from_encoding("WCHAR_T", iconveh_question_mark,
			(const char *)ws, ws_len * sizeof(wchar_t), NULL, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	ret = yπio_printctx_putπ(t, result, length);
	free(result);
	return ret;
~)m4_dnl;
}

int _yΩIO_print_wchar(yπio_printctx_t *t) {
	const wchar_t wc[] = { yπio_printctx_va_arg(t, wchar_t), L'\0' };
	return _yΩIO_print_wcharpnt_in(t, wc);
}

int _yΩIO_print_wcharpnt(yπio_printctx_t *t) {
	return _yΩIO_print_wcharpnt_in(t, yπio_printctx_va_arg(t, wchar_t *));
}

int _yΩIO_print_constwcharpnt(yπio_printctx_t *t) {
	// TODO: precision
	return _yΩIO_print_wcharpnt_in(t, yπio_printctx_va_arg(t, const wchar_t *));
}

