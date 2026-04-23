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

int YYIO_print_wchar(yio_printctx_t *t) {
	const wchar_t wc = yio_printctx_va_arg(t, wchar_t);
	int ret = yio_printctx_init(t);
	if (ret) return ret;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	const char *dst = NULL;
	size_t dst_len = 0;
	switch (pf->type) {
	case '\0':
	case 'c':
		ret = YYIO_strconv_wstr_to_str(&wc, 1, &dst, &dst_len);
		if (ret) return ret;
		ret = yio_printctx_put(t, dst, dst_len);
		YYIO_strconv_free_wstr_to_str(&wc, dst);
		break;
	case 'b':
	case 'B':
	case 'd':
	case 'o':
	case 'x':
	case 'X':
#if WCHAR_MAX <= UINT_MAX
		ret = YYIO_print_uint_in(t, wc, false);
#elif WCHAR_MAX <= ULONG_MAX
		ret = YYIO_print_ulong_in(t, wc, false);
#else
		ret = YYIO_print_ullong_in(t, wc, false);
#endif
		break;
	default:
		ret = YIO_ERROR_INVALID_TYPE;
	}
	return ret;
}

int YYIO_print_constwcharpnt(yio_printctx_t *t) {
	const wchar_t *const ws1 = yio_printctx_va_arg(t, const wchar_t *);
	const wchar_t *const ws = ws1 == NULL ? L"(null)" : ws1;
	int ret = yio_printctx_init(t);
	if (ret) return ret;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	const size_t ws_len = pf->precision >= 0 ? YYIO_wstrnlen(ws, pf->precision) : wcslen(ws);
	const char *dst = NULL;
	size_t dst_len = 0;
	ret = YYIO_strconv_wstr_to_str(ws, ws_len, &dst, &dst_len);
	if (ret) return ret;
	ret = yio_printctx_put(t, dst, dst_len);
	YYIO_strconv_free_wstr_to_str(ws, dst);
	return ret;
}

