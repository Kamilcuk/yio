/**
 * @file
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
#define _POSIX_C_SOURCE  200809L
#include "private.h"
#if YIO_HAS_WCHAR_H
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifndef __has_feature
#define __has_feature(x) 0
#endif

static int wstr_to_yyiostring(const wchar_t *ws, size_t ws_maxlen, YYIO_string *out) {
	mbstate_t state;
	memset(&state, 0, sizeof(state));
	const size_t mb_cur_max = MB_CUR_MAX;
#if YYIO_HAS_wcsnrtombs
	const wchar_t *psrc = ws;
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
	// Calculate true max_len to avoid passing (size_t)-1 to wcsnrtombs.
	// AddressSanitizer's interceptor for wcsnrtombs incorrectly flags (size_t)-1
	// as a negative-size-param error. This workaround prevents the crash.
	if (ws_maxlen == (size_t)-1) {
		ws_maxlen = wcsnlen(ws, (size_t)-1);
	}
#endif
	while (psrc != NULL && ws_maxlen > 0) {
		if (YYIO_string_free_size(out) < mb_cur_max) {
			const int err = YYIO_string_reserve_more(out, mb_cur_max);
			if (err) return err;
		}
		const size_t cap = YYIO_string_free_size(out);
		const wchar_t *const psrc_old = psrc;
		const size_t r = wcsnrtombs(YYIO_string_data(out) + YYIO_string_len(out), &psrc, ws_maxlen, cap, &state);
		if (r == (size_t)-1) return YIO_ERROR_WCRTOMB;
		YYIO_string_set_used(out, YYIO_string_len(out) + r);
		if (psrc == NULL) break; // L'\0' reached
		const size_t consumed = (size_t)(psrc - psrc_old);
		if (consumed >= ws_maxlen) break;
		ws_maxlen -= consumed;
		if (r == 0 && consumed == 0) {
			// Buffer too small to even convert one character? Should not happen due to caching.
			const int err = YYIO_string_reserve_more(out, mb_cur_max);
			if (err) return err;
		}
	}
#else
	for (size_t i = 0; i < ws_maxlen && ws[i] != L'\0'; ++i) {
		if (YYIO_string_free_size(out) < mb_cur_max) {
			const int err = YYIO_string_reserve_more(out, mb_cur_max);
			if (err) return err;
		}
		const size_t r = wcrtomb(YYIO_string_data(out) + YYIO_string_len(out), ws[i], &state);
		if (r == (size_t)-1) return YIO_ERROR_WCRTOMB;
		YYIO_string_set_used(out, YYIO_string_len(out) + r);
	}
#endif
	return 0;
}

int YYIO_print_wchar(yio_printctx_t *t) {
	const wchar_t wc = yio_printctx_va_arg(t, wchar_t);
	int ret = yio_printctx_init(t);
	if (ret) return ret;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	switch (pf->type) {
	case '\0':
	case 'c': {
		char buf[MB_LEN_MAX];
		mbstate_t state;
		memset(&state, 0, sizeof(state));
		const size_t r = wcrtomb(buf, wc, &state);
		if (r == (size_t)-1) return YIO_ERROR_WCRTOMB;
		return yio_printctx_put(t, buf, r);
	}
	case 'b':
	case 'B':
	case 'd':
	case 'o':
	case 'x':
	case 'X':
#if WCHAR_MAX <= UINT_MAX
		ret = YYIO_print_uint_in(t, (unsigned int)wc, false);
#elif WCHAR_MAX <= ULONG_MAX
		ret = YYIO_print_ulong_in(t, (unsigned long)wc, false);
#else
		ret = YYIO_print_ullong_in(t, (unsigned long long)wc, false);
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
	if (pf->type != '\0' && pf->type != 's') return YIO_ERROR_INVALID_TYPE;
	const size_t ws_maxlen = yio_precision_get_default(pf->precision, SIZE_MAX);
	YYIO_string out;
	YYIO_string_init(&out);
	ret = wstr_to_yyiostring(ws, ws_maxlen, &out);
	if (ret == 0) {
		ret = yio_printctx_put(t, YYIO_string_data(&out), YYIO_string_len(&out));
	}
	YYIO_string_free(&out);
	return ret;
}

#endif
