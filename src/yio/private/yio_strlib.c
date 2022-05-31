/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "private.h"
#include "yio_strlib.h"
#if YYIO_HAS_UNISTRING
#include <uniconv.h>
#include <unistr.h>
#endif
#include <string.h>

#ifndef YYIO_HAS_UNISTRING
#error
#endif

/* ------------------------------------------------------------------------- */

size_t YYIO_strnlen(const char *str, size_t maxlen) {
#if YYIO_HAS_strnlen
	return strnlen(str, maxlen);
#else
	const char *str0 = str;
	while (maxlen-- > 0 && *str) {
		++str;
	}
	return str - str0;
#endif
}

#if YIO_HAS_WCHAR_H
size_t YYIO_wstrnlen(const wchar_t *str, size_t maxlen) {
#if YYIO_HAS_strnlen
	return wcsnlen(str, maxlen);
#else
	const wchar_t *str0 = str;
	while (maxlen-- > 0 && *str) {
			++str;
		}
		return str - str0;
#endif
}
#endif

#if YIO_HAS_UCHAR_H

size_t YYIO_c16strnlen(const char16_t *str, size_t maxlen) {
#if YYIO_HAS_UNISTRING
	return u16_strnlen(str, maxlen);
#else
	const char16_t *str0 = str;
	while (maxlen-- && *str) {
		++str;
	}
	return str - str0;
#endif
}

size_t YYIO_ustrnlen(const char32_t *str, size_t maxlen) {
#if YYIO_HAS_UNISTRING
	return u32_strnlen(str, maxlen);
#else
	const char32_t *str0 = str;
	while (maxlen-- && *str) {
		++str;
	}
	return str - str0;
#endif
}

#endif

/* YYIO_strconv_* -------------------------------------------------------- */

/**
 * Convert one type to another when characters from src have
 * the same representation as character in dst.
 */
#define YYIO_RETURN_STRCONV_SAME(src_type, dst_type, src, src_len, dst, dst_len)  do { \
	dst_type * const _out = malloc(sizeof(*_out) * src_len); /* NOLINT(bugprone-macro-parentheses) */ \
	if (_out == NULL) return YIO_ERROR_ENOMEM; \
	{ \
		dst_type * _iout = _out; /* NOLINT(bugprone-macro-parentheses) */ \
		const src_type * _iin = src; \
		for (size_t i = 0; i < src_len; ++i) { /* NOLINT(bugprone-macro-parentheses) */ \
			*_iout++ = *_iin++; \
		} \
	} \
	*dst = _out; /* NOLINT(bugprone-macro-parentheses) */ \
	if (dst_len) { \
		*dst_len = src_len; /* NOLINT(bugprone-macro-parentheses) */ \
	} \
	return 0; \
} while(0)

/* strconv_str_to_* -------------------------------------------------------------------- */

#if YIO_HAS_WCHAR_H

static inline
int YYIO_strconv_str_to_wstr_count(const char *mb, size_t mb_len, size_t *wc_len) {
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	// mbsrtowcs(NULL, &src, mb_len, &ps); can't be used!
	// When first argument is NULL, then mb_len is ignored, and mbsrtowcs
	// just scans until a terminating character. We have to loop.
	size_t out_len = 0;
	for (const char *src = mb, *srcend = &mb[mb_len]; src != srcend;) {
		const size_t tt = mbrtowc(NULL, src, srcend - src, &ps);
		switch (tt) {
		case (size_t)-1: return YIO_ERROR_MBRTOWC;
		case (size_t)-2: return YIO_ERROR_MBRTOWC_2;
		}
		assert(tt != 0);
		src += tt;
		out_len++;
	}
	*wc_len = out_len;
	return 0;
}

static inline
int YYIO_strconv_str_to_wstr_conv(const char *mb, wchar_t *out, size_t out_len) {
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	const size_t len2 = mbsrtowcs(out, &mb, out_len, &ps);
	if (len2 == (size_t)-1 || len2 == (size_t)-2) {
		return YIO_ERROR_MBRTOWC;
	}
	// dbgln("%zu %zu `%ls` `%s` %zu", len2, out_len, out, mb, mb_len);
	assert(len2 == out_len);
	return 0;
}

int YYIO_strconv_str_to_wstr(const char *mb, size_t mb_len, const wchar_t **wc, size_t *wc_len) {
	if (mb_len == 0) {
		*wc = NULL;
		if (wc_len) *wc_len = 0;
		return 0;
	}
	size_t out_len = 0;
	int ret = YYIO_strconv_str_to_wstr_count(mb, mb_len, &out_len);
	if (ret) return ret;
	wchar_t *const out = malloc(sizeof(wchar_t) * out_len);
	if (out == NULL) return YIO_ERROR_ENOMEM;
	ret = YYIO_strconv_str_to_wstr_conv(mb, out, out_len);
	if (ret) {
		free(out);
		return ret;
	}
	*wc = out;
	if (wc_len) *wc_len = out_len;
	return ret;
}

#endif

#if YIO_HAS_UCHAR_H

{% macro j_str_to_ustr() %}
#line

static inline
int YYIO_strconv_str_to_ustr_mbrtowc32_in(const char *in, size_t in_len, char32_t *pc32, size_t *plength) {
	const char *in_end = in + in_len;
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	size_t length = 0;
	for (const char *ii = in; in != in_end; ++ii) {
		const size_t len = mbrtoc32(pc32, ii, in_end - ii, &ps);
		switch (len) {
		case (size_t)-1: return YIO_ERROR_MBRTOC32;
		case (size_t)-2: return YIO_ERROR_MBRTOC32_2;
		case (size_t)-3: return YIO_ERROR_MBRTOC32_3;
		}
		length += len == 0 ? 1 : len;
		if (pc32) ++pc32;
	}
	*plength = length;
	return 0;
}

static inline
int YYIO_strconv_str_to_ustr_mbrtowc32(const char *in, size_t in_len, const char32_t **pout, size_t *pout_len) {
	size_t length = 0;
	int ret = YYIO_strconv_str_to_ustr_mbrtowc32_in(in, in_len, NULL, &length);
	if (ret) return ret;
	assert(length != 0);
	char32_t *const out = malloc(sizeof(*out) * length);
	if (out == NULL) return YIO_ERROR_ENOMEM;
	size_t length2 = 0;
	ret = YYIO_strconv_str_to_ustr_mbrtowc32_in(in, in_len, out, &length2);
	if (ret) {
		free(out);
		return ret;
	}
	assert(length2 == length);
	*pout = out;
	if (pout_len != NULL) *pout_len = length;
	return 0;
}

int YYIO_strconv_str_to_ustr(const char *in, size_t in_len, const char32_t **pout, size_t *pout_len) {
	if (in_len == 0) {
		*pout = NULL;
		if (pout_len != NULL) *pout_len = 0;
		return 0;
	}
#if __STDC_UTF_32__ && YYIO_HAS_UNISTRING
	size_t length = 0;
	uint32_t *buf = u32_conv_from_encoding(locale_charset(), iconveh_question_mark,
			in, in_len, NULL, NULL, &length);
	if (buf == NULL) {
		return YIO_ERROR_U32_CONV_FROM_ENCODING;
	}
	*pout = buf;
	if (pout_len != NULL) *pout_len = length;
	return 0;
#else
	return YYIO_strconv_str_to_ustr_mbrtowc32(in, in_len, pout, pout_len);
#endif
}

{% endmacro %}
#line
{{ j_str_to_ustr() | replace('32', '16') | replace('ustr', 'c16str') }}
{{ j_str_to_ustr() }}

#endif // YIO_HAS_UCHAR_H

/* strconv_wstr_to_* -------------------------------------------------------------------- */

#if YIO_HAS_WCHAR_H

static inline
int YYIO_strconv_wstr_to_str_count(const wchar_t *wc, size_t wc_len, size_t *plength) {
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	size_t out_len = 0;
	for (const wchar_t *src = wc, *wc_end = wc + wc_len; src != wc_end; ++src) {
		const size_t len = wcrtomb(NULL, *src, &ps);
		if (len == (size_t)-1) return YIO_ERROR_WCRTOMB;
		assert(len != 0);
		out_len += len;
	}
	*plength = out_len;
	return 0;
}

static inline
int YYIO_strconv_wstr_to_str_conv(const wchar_t *wc, size_t wc_len, char *out, size_t out_len) {
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	const size_t len2 = wcsrtombs(out, &wc, wc_len, &ps);
	if (len2 == (size_t)-1) return YIO_ERROR_WCRTOMB;
	// dbgln("%zu %zu", len2, len);
	assert(len2 == out_len);
	return 0;
}


int YYIO_strconv_wstr_to_str(const wchar_t *wc, size_t wc_len, const char **mb, size_t *mb_len) {
	if (wc_len == 0) {
		*mb = NULL;
		if (mb_len != NULL) *mb_len = 0;
		return 0;
	}
	size_t out_len = 0;
	int ret = YYIO_strconv_wstr_to_str_count(wc, wc_len, &out_len);
	if (ret) return ret;
	assert(out_len != 0);
	char *const out = malloc(sizeof(*out) * out_len);
	if (out == NULL) return YIO_ERROR_ENOMEM;
	ret = YYIO_strconv_wstr_to_str_conv(wc, wc_len, out, out_len);
	if (ret) {
		free(out);
		return ret;
	}
	*mb = out;
	if (mb_len != NULL) *mb_len = out_len;
	return ret;
}

#if YIO_HAS_UCHAR_H

{% macro j_wstr_to_ustr() %}
#line

int YYIO_strconv_wstr_to_ustr(const wchar_t *src, size_t src_len, const char32_t **dst, size_t *dst_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	YYIO_RETURN_STRCONV_SAME(wchar_t, char32_t, src, src_len, dst, dst_len);
#else
	if (!src_len) {
		*dst = NULL;
		if (dst_len) {
			*dst_len = 0;
		}
		return 0;
	}

	char *mb = NULL;
	size_t mb_len = 0;
	int ret = YYIO_strconv_wstr_to_str(src, src_len, &mb, &mb_len);
	if (ret) return ret;
	ret = YYIO_strconv_str_to_c32str(mb, mb_len, dst, dst_len);
	YYIO_strconv_free_wstr_to_str(src, mb);
	return ret;
#endif
}

{% endmacro %}
{{ j_wstr_to_ustr() | replace('32', '16') | replace('ustr', 'c16str') }}
{{ j_wstr_to_ustr() }}
#line

#endif // YIO_HAS_UCHAR_H

#endif // YIO_HAS_WCHAR_H

/* strconv_ustr_to_* -------------------------------------------------------------------- */

#if YIO_HAS_UCHAR_H

{% macro j_uchar_functions() %}
#line

static inline
int YYIO_strconv_ustr_to_str_conv(const char32_t *c32, size_t c32_len, char *out, size_t *pout_len) {
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	size_t out_len = 0;
	for (const char32_t *ii = c32, *c32_end = c32 + c32_len; ii != c32_end; ++ii) {
		size_t r = c32rtomb(out, *ii, &ps);
		if (r == (size_t)-1) return YIO_ERROR_C32RTOMB;
		assert(r != 0);
		out_len += r;
		if (out) ++out;
	}
	*pout_len = out_len;
	return 0;
}


int YYIO_strconv_ustr_to_str(const char32_t *c32, size_t c32_len, const char **mb, size_t *mb_len) {
	if (!c32_len) {
		*mb = NULL;
		if (mb_len) *mb_len = 0;
		return 0;
	}
	size_t out_len = 0;
	int ret = YYIO_strconv_ustr_to_str_conv(c32, c32_len, NULL, &out_len);
	if (ret) return ret;
	assert(out_len != 0);
	char *const out = malloc(sizeof(*out) * out_len);
	if (out == NULL) return YIO_ERROR_ENOMEM;
	size_t out_len2 = 0;
	ret = YYIO_strconv_ustr_to_str_conv(c32, c32_len, out, &out_len2);
	if (ret) {
		free(out);
		return ret;
	}
	assert(out_len == out_len2);
	*mb = out;
	if (mb_len) *mb_len = out_len;
	return 0;
}

#if YIO_HAS_WCHAR_H

static inline
int YYIO_strconv_ustr_to_wstr_NE(const char32_t *c32, size_t c32_len, const wchar_t **wc, size_t *wc_len) {
	if (!c32_len) {
		*wc = NULL;
		if (wc_len) *wc_len = 0;
		return 0;
	}
	const char *mb = NULL;
	size_t mb_len = 0;
	int ret = YYIO_strconv_ustr_to_str(c32, c32_len, &mb, &mb_len);
	if (ret) return ret;
	if (mb == NULL) {
		*wc = NULL;
		if (wc_len) *wc_len = 0;
		return 0;
	}
	ret = YYIO_strconv_str_to_wstr(mb, mb_len, wc, wc_len);
	free((void*)mb); // cppcheck-suppress cert-EXP05-C
	return ret;
}

static inline
int YYIO_strconv_ustr_to_wstr_EQ(const char32_t *src, size_t src_len, const wchar_t **dst, size_t *dst_len) {
	YYIO_RETURN_STRCONV_SAME(char32_t, wchar_t, src, src_len, dst, dst_len);
}

int YYIO_strconv_ustr_to_wstr(const char32_t *c32, size_t c32_len, const wchar_t **wc, size_t *wc_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	return YYIO_strconv_ustr_to_wstr_EQ(c32, c32_len, wc, wc_len);
#else
	return YYIO_strconv_ustr_to_wstr_NE(c32, c32_len, wc, wc_len);
#endif
}
#endif // YIO_HAS_WCHAR_H
{% endmacro %}
{{ j_uchar_functions() | replace('32', '16') | replace('ustr', 'c16str') }}
{{ j_uchar_functions() }}
#line

static inline
int YYIO_strconv_c16str_to_ustr_no_unistring(const char16_t *c16, size_t c16_len, const char32_t **c32, size_t *c32_len) {
	const char *mb;
	size_t mb_len;
	int ret = YYIO_strconv_c16str_to_str(c16, c16_len, &mb, &mb_len);
	if (ret) return ret;
	if (mb == NULL) {
		*c32 = NULL;
		if (c32_len) *c32_len = 0;
		return 0;
	}
	ret = YYIO_strconv_str_to_ustr(mb, mb_len, c32, c32_len);
	free((void*)mb); // cppcheck-suppress cert-EXP05-C
	return ret;
}

static inline
int YYIO_strconv_ustr_to_c16str_no_unistring(const char32_t *c32, size_t c32_len, const char16_t **c16, size_t *c16_len) {
	const char *mb;
	size_t mb_len;
	int ret = YYIO_strconv_ustr_to_str(c32, c32_len, &mb, &mb_len);
	if (ret) return ret;
	if (mb == NULL) {
		*c16 = NULL;
		if (c16_len) *c16_len = 0;
		return 0;
	}
	ret = YYIO_strconv_str_to_c16str(mb, mb_len, c16, c16_len);
	free((void*)mb); // cppcheck-suppress cert-EXP05-C
	return ret;
}

int YYIO_strconv_c16str_to_ustr(const char16_t *c16, size_t c16_len, const char32_t **c32, size_t *c32_len) {
	if (c16_len == 0) {
		*c32 = NULL;
		if (c32_len != NULL) *c32_len = 0;
		return 0;
	}
#if __STDC_UTF_16__ && __STDC_UTF_32__ && __STDC_ISO_10646__ && YYIO_HAS_UNISTRING
	size_t length = 0;
	uint32_t *result = u16_to_u32(c16, c16_len, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	*c32 = result;
	if (c32_len) {
		*c32_len = length;
	}
	return 0;
#else
	return YYIO_strconv_ustr_to_c16str_no_unistring(c16, c16_len, c32, c32_len);
#endif
}

int YYIO_strconv_ustr_to_c16str(const char32_t *c32, size_t c32_len, const char16_t **c16, size_t *c16_len) {
	if (c32_len == 0) {
		*c16 = NULL;
		if (c16_len) *c16_len = 0;
		return 0;
	}
#if __STDC_UTF_16__ && __STDC_UTF_32__ && __STDC_ISO_10646__ && YYIO_HAS_UNISTRING
	size_t length = 0;
	uint16_t *result = u32_to_u16(c32, c32_len, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	*c16 = result;
	if (c16_len) {
		*c16_len = length;
	}
	return 0;
#else
	return YYIO_strconv_c16str_to_ustr_no_unistring(c32, c32_len, c16, c16_len);
#endif
}

#endif // YIO_HAS_UCHAR_H

