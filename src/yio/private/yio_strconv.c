/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "yio_strconv.h"
#ifdef _yIO_HAS_UNISTRING
#include <uniconv.h>
#include <unistr.h>
#endif

#ifdef _yIO_HAS_WCHAR_H

int _yIO_conv_wcs_to_mbs(const wchar_t *wcs, size_t wcs_len, char **mb, size_t *mb_len) {
	int ret = 0;

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	const wchar_t *src = wcs;
	size_t out_len = 0;
	for (size_t i = 0; i < wcs_len; ++i) {
		const size_t len = wcrtomb(NULL, *src, &ps);
		if (len == (size_t)-1) {
			ret = YIO_ERROR_WCTOMB_ERR;
			goto ERR_wcsrtombs_1;
		}
		src++;
		out_len += len;
	}

	char * const out = malloc(sizeof(*out) * out_len);
	if (out == NULL) { ret = YIO_ERROR_ENOMEM; goto ERR_MALLOC; }

	memset(&ps, 0, sizeof(ps));
	src = wcs;
	const size_t len2 = wcsrtombs(out, &src, wcs_len, &ps);
	if (len2 == (size_t)-1) { ret = YIO_ERROR_WCTOMB_ERR; goto ERR_wcsrtombs_2; }

	*mb = out;
	if (mb_len) {
		*mb_len = out_len;
	}
	return ret;

	ERR_wcsrtombs_2:
	free(out);
	ERR_MALLOC:
	ERR_wcsrtombs_1:
	return ret;
}

int _yIO_conv_mbs_to_wcs(const char *mb, size_t mb_len, wchar_t **wcs, size_t *wcs_len) {
	int ret = 0;
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	size_t out_len = 0;
	const char *src = mb;
	for (size_t i = 0; i < mb_len; ++i) {
		const size_t t = mbrtowc(NULL, src, mb_len - i, &ps);
		if (t == (size_t)-1 || t == (size_t)-2) {
			ret = YIO_ERROR_MBTOWC_ERR;
			goto ERR_mbrtowc;
		}
		src += t;
		out_len++;
	}

	wchar_t *out = malloc(sizeof(wchar_t) * out_len);
	if (out == NULL) { ret = YIO_ERROR_ENOMEM; goto ERR_MALLOC; }

	memset(&ps, 0, sizeof(ps));
	src = mb;
	const size_t len2 = mbsrtowcs(out, &src, mb_len, &ps);
	if (len2 == (size_t)-1 || len2 == (size_t)-2) {
		ret = YIO_ERROR_WCTOMB_ERR;
		goto ERR_mbsrtowcs;
	}

	*wcs = out;
	if (wcs_len) {
		*wcs_len = out_len;
	}
	return ret;

	ERR_mbsrtowcs:
	free(out);
	ERR_MALLOC:
	ERR_mbrtowc:
	return ret;
}

#endif // _yIO_HAS_WCHAR_H

#ifdef _yIO_HAS_UCHAR_H

m4_define(`m4_uchar_functions~, `m4_dnl;

int _yIO_conv_c32s_to_mbs(const char32_t *c32s, size_t c32s_len, char **mb, size_t *mb_len) {
	char *out = malloc(MB_CUR_MAX);
	if (out == NULL) return YIO_ERROR_ENOMEM;

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	char *assign = out;
	size_t i_len = c32s_len;
	for (const char32_t *i = c32s; i_len--; ++i) {
		size_t r = c32rtomb(assign, *i, &ps);
		if (r == (size_t)-1) {
			free(out);
			return YIO_ERROR_c32rtomb;
		}
		assign += r;
	}
	const size_t out_len = assign - out;

	*mb = out;
	if (mb_len) {
		*mb_len = out_len;
	}
	return 0;
}

static inline
int _yIO_conv_c32s_to_wcs_NE(const char32_t *c32s, size_t c32s_len, wchar_t **wcs, size_t *wcs_len) {
	int ret = 0;

	char *mb;
	size_t mb_len;
	ret = _yIO_conv_c32s_to_mbs(c32s, c32s_len, &mb, &mb_len);
	if (ret) {
		goto ERR_CONV_C32_TO_MB;
	}

	wchar_t * const out = malloc(MB_CUR_MAX);
	if (out == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto ERR_MALLOC;
	}

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	const char *pmb = mb;
	const size_t r = mbsrtowcs(out, &pmb, mb_len, &ps);
	if (r == (size_t)-1 || r == (size_t)-2) {
		ret = YIO_ERROR_MBTOWC_ERR;
		goto ERR_MBSRTOWCS;
	}
	free(mb);

	*wcs = out;
	if (wcs_len) {
		*wcs_len = r;
	}
	return ret;

	ERR_MBSRTOWCS:
	free(out);
	ERR_MALLOC:
	free(mb);
	ERR_CONV_C32_TO_MB:
	return ret;
}

static inline
int _yIO_conv_c32s_to_wcs_EQ(const char32_t *c32s, size_t c32s_len, wchar_t **wcs, size_t *wcs_len) {
	wchar_t *out = malloc(c32s_len * sizeof(*out));
	if (out == NULL) {
		return YIO_ERROR_ENOMEM;
	}
	for (size_t i = 0; i < c32s_len; ++i) {
		out[i] = c32s[i];
	}
	*wcs = out;
	if (wcs_len) {
		*wcs_len = c32s_len;
	}
	return 0;
}

int _yIO_conv_c32s_to_wcs(const char32_t *c32s, size_t c32s_len, wchar_t **wcs, size_t *wcs_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	return _yIO_conv_c32s_to_wcs_EQ(c32s, c32s_len, wcs, wcs_len);
#else
	return _yIO_conv_c32s_to_wcs_NE(c32s, c32s_len, wcs, wcs_len);
#endif
}

~)m4_dnl;
m4_dnl Generate char16_t fucntions by replacing 32 to 16
m4_patsubst(m4_uchar_functions(), `32~, `16~)
m4_dnl Then just generate 32
m4_uchar_functions()
m4_dnl;

int _yIO_conv_c16s_to_c32s(const char16_t *c16s, size_t c16s_len, char32_t **c32s, size_t *c32s_len) {
#if __STDC_UTF_16__ && __STDC_UTF_32__ && __STDC_ISO_10646__ && defined _yIO_HAS_UNISTRING
	size_t length;
	uint32_t *result = u16_to_u32(c16s, c16s_len, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	*c32s = result;
	if (c32s_len) {
		*c32s_len = length;
	}
	return 0;
#else
#error TODO
#endif
}

int _yIO_conv_mbs_to_c32s(const char *mbs, size_t mbs_len, char32_t **c32s, size_t *c32s_len) {
#ifdef _yIO_HAS_UNISTRING
	size_t length = 0;
	uint32_t *buf = u32_conv_from_encoding(locale_charset(), iconveh_question_mark,
			mbs, mbs_len, NULL, NULL, &length);
	if (buf == NULL) return YIO_ERROR_ENOMEM;
	*c32s = buf;
	if (c32s_len) {
		*c32s_len = length;
	}
	return 0;
#else
#error TODO: with mbtoc32
#endif
}

#endif // _yIO_HAS_UCHAR_H
