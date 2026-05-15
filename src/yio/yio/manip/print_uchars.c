/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include "../../private/yio_buf.h"
#if YIO_HAS_UCHAR_H
#include <uchar.h>
#include <string.h>
#include <limits.h>

static int c16str_to_yyiostring(const char16_t *str, size_t maxlen, YIO_buf *out) {
	mbstate_t state;
	memset(&state, 0, sizeof(state));
	const size_t mb_cur_max = MB_CUR_MAX;
	for (size_t i = 0; i < maxlen && str[i] != u'\0'; ++i) {
		if (YIO_buf_free_size(out) < mb_cur_max) {
			const int err = YIO_buf_reserve_more(out, mb_cur_max);
			if (err) return err;
		}
		const size_t r = c16rtomb(YIO_buf_data(out) + YIO_buf_len(out), str[i], &state);
		if (r == (size_t)-1) return YIO_ERROR_WCRTOMB;
		if (r != (size_t)-2 && r != (size_t)-3) {
			YIO_buf_set_used(out, YIO_buf_len(out) + r);
		}
	}
	return 0;
}

static int ustr_to_yyiostring(const char32_t *str, size_t maxlen, YIO_buf *out) {
	mbstate_t state;
	memset(&state, 0, sizeof(state));
	const size_t mb_cur_max = MB_CUR_MAX;
	for (size_t i = 0; i < maxlen && str[i] != U'\0'; ++i) {
		if (YIO_buf_free_size(out) < mb_cur_max) {
			const int err = YIO_buf_reserve_more(out, mb_cur_max);
			if (err) return err;
		}
		const size_t r = c32rtomb(YIO_buf_data(out) + YIO_buf_len(out), str[i], &state);
		if (r == (size_t)-1) return YIO_ERROR_WCRTOMB;
		if (r != (size_t)-2 && r != (size_t)-3) {
			YIO_buf_set_used(out, YIO_buf_len(out) + r);
		}
	}
	return 0;
}

int YIO_print_constchar16pnt(yio_printctx_t *t) {
	const char16_t *const str1 = yio_printctx_va_arg(t, const char16_t *);
	const char16_t *const str = str1 == NULL ? u"(null)" : str1;
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	if (pf->type != '\0' && pf->type != 's') return YIO_ERROR_INVALID_TYPE;
	const size_t maxlen = yio_precision_get_default(pf->precision, SIZE_MAX);
	YIO_buf out;
	YIO_buf_init(&out);
	err = c16str_to_yyiostring(str, maxlen, &out);
	if (err == 0) {
		err = yio_printctx_put(t, YIO_buf_data(&out), YIO_buf_len(&out));
	}
	YIO_buf_fini(&out);
	return err;
}

int YIO_print_constchar32pnt(yio_printctx_t *t) {
	const char32_t *const str1 = yio_printctx_va_arg(t, const char32_t *);
	const char32_t *const str = str1 == NULL ? U"(null)" : str1;
	int err = yio_printctx_init(t);
	if (err) return err;
	const struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	if (pf->type != '\0' && pf->type != 's') return YIO_ERROR_INVALID_TYPE;
	const size_t maxlen = yio_precision_get_default(pf->precision, SIZE_MAX);
	YIO_buf out;
	YIO_buf_init(&out);
	err = ustr_to_yyiostring(str, maxlen, &out);
	if (err == 0) {
		err = yio_printctx_put(t, YIO_buf_data(&out), YIO_buf_len(&out));
	}
	YIO_buf_fini(&out);
	return err;
}

#endif
