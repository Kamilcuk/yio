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

int YYΩIO_print_wchar(yπio_printctx_t *t) {
	const wchar_t wc = yπio_printctx_va_arg(t, wchar_t);
	int ret = yπio_printctx_init(t);
	if (ret) return ret;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const TCHAR *dst = NULL;
	size_t dst_len = 0;
	switch (pf->type) {
	case TC('\0'):
	case TC('c'):
		ret = YYIO_strconv_wstr_to_πstr(&wc, 1, &dst, &dst_len);
		if (ret) return ret;
		ret = yπio_printctx_putπ(t, dst, dst_len);
		YYIO_strconv_free_wstr_to_πstr(&wc, dst);
		break;
	case TC('b'):
	case TC('B'):
	case TC('d'):
	case TC('o'):
	case TC('x'):
	case TC('X'):
#if WCHAR_MAX <= UINT_MAX
		ret = YYΩIO_print_uint_in(t, wc, false);
#elif WCHAR_MAX <= ULONG_MAX
		ret = YYΩIO_print_ulong_in(t, wc, false);
#else
		ret = YYΩIO_print_ullong_in(t, wc, false);
#endif
		break;
	default:
		ret = YIO_ERROR_INVALID_TYPE;
	}
	return ret;
}

int YYΩIO_print_constwcharpnt(yπio_printctx_t *t) {
	const wchar_t *const ws1 = yπio_printctx_va_arg(t, const wchar_t *);
	const wchar_t *const ws = ws1 == NULL ? L"(null)" : ws1;
	int ret = yπio_printctx_init(t);
	if (ret) return ret;
	const struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	const size_t ws_len = pf->precision >= 0 ? YYIO_wstrnlen(ws, pf->precision) : wcslen(ws);
	const TCHAR *dst = NULL;
	size_t dst_len = 0;
	ret = YYIO_strconv_wstr_to_πstr(ws, ws_len, &dst, &dst_len);
	if (ret) return ret;
	ret = yπio_printctx_putπ(t, dst, dst_len);
	YYIO_strconv_free_wstr_to_πstr(ws, dst);
	return ret;
}

