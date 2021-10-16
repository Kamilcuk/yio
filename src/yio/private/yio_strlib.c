/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "yio_strlib.h"
#if _yIO_HAS_UNISTRING
#include <uniconv.h>
#include <unistr.h>
#endif

#ifndef _yIO_HAS_UNISTRING
#error
#endif

#ifdef __CDT_PARSER__
#define _yIO_strconv_str_to_πstr
#define _yIO_strconv_wstr_to_πstr
#define _yIO_strconv_c16str_to_πstr
#define _yIO_strconv_ustr_to_πstr
#define _yIO_strconv_free_str_to_πstr
#define _yIO_strconv_free_wstr_to_πstr
#define _yIO_strconv_free_c16str_to_πstr
#define _yIO_strconv_free_ustr_to_πstr
#endif

/* ------------------------------------------------------------------------- */

size_t _yIO_strnlen(const char *str, size_t maxlen) {
#if _yIO_HAS_strnlen
	return strnlen(str, maxlen);
#else
	const char *str0 = str;
	while (maxlen-- > 0 && *str) {
		++str;
	}
	return str - str0;
#endif
}

#if _yIO_HAS_WCHAR_H
size_t _yIO_wstrnlen(const wchar_t *str, size_t maxlen) {
#if _yIO_HAS_strnlen
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

#if _yIO_HAS_UCHAR_H

size_t _yIO_c16strnlen(const char16_t *str, size_t maxlen) {
#if _yIO_HAS_UNISTRING
	return u16_strnlen(str, maxlen);
#else
	const char16_t *str0 = str;
	while (maxlen-- && *str) {
		++str;
	}
	return str - str0;
#endif
}

size_t _yIO_ustrnlen(const char32_t *str, size_t maxlen) {
#if _yIO_HAS_UNISTRING
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

/* _yIO_strconv_* -------------------------------------------------------- */

/**
 * Convert one type to another when characters from src have
 * the same representation as character in dst.
 */
#define _yIO_RETURN_STRCONV_SAME(src_type, dst_type, src, src_len, dst, dst_len)  do { \
	dst_type * const _out = malloc(sizeof(*_out) * src_len); \
	if (_out == NULL) { \
		return YIO_ERROR_ENOMEM; \
	} else { \
		{ \
			dst_type * _iout = _out; \
			const src_type * _iin = src; \
			for (size_t i = 0; i < src_len; ++i) { \
				*_iout++ = *_iin++; \
			} \
		} \
		*dst = _out; \
		if (dst_len) { \
			*dst_len = src_len; \
		} \
		return 0; \
	} \
} while(0)

/**
 * Converts src to dst types by using a type in between.
 * Used to convert ex. utf32 to utf16 by using multibyte between.
 */
#define _yIO_RETURN_STRCONV_FORWARD(middle_type, \
		FUNC_dst_to_middle, \
		FUNC_free_dst_to_middle, \
		FUNC_middle_to_dst)  do { \
	middle_type *mb; \
	size_t mb_len; \
	int ret = FUNC_dst_to_middle(src, src_len, &mb, &mb_len); \
	if (ret) return ret; \
	ret = FUNC_middle_to_dst(mb, mb_len, dst, dst_len); \
	FUNC_free_dst_to_middle(src, mb); \
	return ret; \
} while(0)


/* strconv_str_to_* -------------------------------------------------------------------- */

#if _yIO_HAS_WCHAR_H
int _yIO_strconv_str_to_wstr(const char *mb, size_t mb_len, const wchar_t **wc, size_t *wc_len) {
	if (!mb_len) {
		*wc = NULL;
		if (wc_len) {
			*wc_len = 0;
		}
		return 0;
	}

	int ret = 0;

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	// mbsrtowcs(NULL, &src, mb_len, &ps); can't be used!
	// When first argument is NULL, then mb_len is ignored, and mbsrtowcs
	// just scans until a terminating character. We have to loop.
	size_t out_len = 0;
	const char *src = mb;
	for (size_t i = 0; i < mb_len; ++i) {
		const size_t t = mbrtowc(NULL, src, mb_len - i, &ps);
		if (t == (size_t)-1 || t == (size_t)-2) {
			ret = YIO_ERROR_MBTOWC;
			goto ERR_mbrtowc;
		}
		src += t;
		out_len++;
	}

	wchar_t *out = malloc(sizeof(wchar_t) * out_len);
	if (out == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto ERR_MALLOC;
	}

	memset(&ps, 0, sizeof(ps));
	src = mb;
	const size_t len2 = mbsrtowcs(out, &src, mb_len, &ps);
	if (len2 == (size_t)-1 || len2 == (size_t)-2) {
		ret = YIO_ERROR_MBTOWC;
		goto ERR_mbsrtowcs;
	}
	/*fprintf(stderr, "%zu %zu `%ls` `%s` %zu\n", len2, out_len, out, mb, mb_len);*/
	assert(len2 == out_len);

	*wc = out;
	if (wc_len) {
		*wc_len = out_len;
	}
	return ret;

	ERR_mbsrtowcs:
	free(out);
	ERR_MALLOC:
	ERR_mbrtowc:
	return ret;
}
#endif

#if _yIO_HAS_UCHAR_H

m4_define(«m4_str_to_ustr», «m4_dnl;
int _yIO_strconv_str_to_ustr(const char *mb, size_t mb_len, const char32_t **c32, size_t *c32_len) {
#if __STDC_UTF_32__ && _yIO_HAS_UNISTRING
	size_t length = 0;
	uint32_t *buf = u32_conv_from_encoding(locale_charset(), iconveh_question_mark,
			mb, mb_len, NULL, NULL, &length);
	if (buf == NULL) {
		return YIO_ERROR_U32_CONV_FROM_ENCODING;
	}
	*c32 = buf;
	if (c32_len) {
		*c32_len = length;
	}
	return 0;
#else
#error TODO: with mbtoc32 or install libunistring
	return -1;
#endif
}
»)m4_dnl;

m4_patsubst(m4_patsubst(m4_str_to_ustr(), «32», «16»), «ustr», «c16str») m4_dnl;

m4_str_to_ustr() m4_dnl;

#endif // _yIO_HAS_UCHAR_H

/* strconv_wstr_to_* -------------------------------------------------------------------- */

m4_syncline()
#if _yIO_HAS_WCHAR_H

int _yIO_strconv_wstr_to_str(const wchar_t *wc, size_t wc_len, const char **mb, size_t *mb_len) {
	if (!wc_len) {
		*mb = NULL;
		if (mb_len) {
			*mb_len = 0;
		}
		return 0;
	}

	int ret = 0;

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	const wchar_t *src = wc;
	size_t out_len = 0;
	for (size_t i = 0; i < wc_len; ++i) {
		const size_t len = wcrtomb(NULL, *src, &ps);
		if (len == (size_t)-1) {
			ret = YIO_ERROR_WCTOMB;
			goto ERR_wcrtomb_1;
		}
		src++;
		out_len += len;
	}

	char * const out = malloc(sizeof(*out) * out_len);
	if (out == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto ERR_MALLOC;
	}

	memset(&ps, 0, sizeof(ps));
	src = wc;
	const size_t len2 = wcsrtombs(out, &src, wc_len, &ps);
	if (len2 == (size_t)-1) {
		ret = YIO_ERROR_WCTOMB;
		goto ERR_wcrtomb_2;
	}
	/*fprintf(stderr, "%zu %zu\n", len2, len);*/
	assert(len2 == out_len);

	*mb = out;
	if (mb_len) {
		*mb_len = out_len;
	}
	return ret;

	ERR_wcrtomb_2:
	free(out);
	ERR_MALLOC:
	ERR_wcrtomb_1:
	return ret;
}

#if _yIO_HAS_UCHAR_H

m4_define_function(«m4_wstr_to_ustr», «

int _yIO_strconv_wstr_to_ustr(const wchar_t *src, size_t src_len, const char32_t **dst, size_t *dst_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	_yIO_RETURN_STRCONV_SAME(wchar_t, char32_t, src, src_len, dst, dst_len);
#else
	if (!src_len) {
		*dst = NULL;
		if (dst_len) {
			*dst_len = 0;
		}
		return 0;
	}

	char *mb; size_t mb_len;
	int ret = _yIO_strconv_wstr_to_str(src, src_len, &mb, &mb_len);
	if (ret) return ret;
	ret = _yIO_strconv_str_to_c32str(mb, mb_len, dst, dst_len);
	_yIO_strconv_free_wstr_to_str(src, mb);
	return ret;
#endif
}

»)m4_dnl;
m4_patsubst(m4_patsubst(m4_wstr_to_ustr(), «32», «16»), «ustr», «c16str») m4_dnl;
m4_wstr_to_ustr() m4_dnl;

#endif // _yIO_HAS_UCHAR_H

#endif // _yIO_HAS_WCHAR_H

/* strconv_ustr_to_* -------------------------------------------------------------------- */

#if _yIO_HAS_UCHAR_H

m4_define(«m4_uchar_functions», «m4_dnl;
»m4_syncline()«

int _yIO_strconv_ustr_to_str(const char32_t *c32, size_t c32_len, const char **mb, size_t *mb_len) {
	if (!c32_len) {
		*mb = NULL;
		if (mb_len) {
			*mb_len = 0;
		}
		return 0;
	}

	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));

	size_t iout_len = 0;
	{
		size_t i_len = c32_len;
		for (const char32_t *i = c32; i_len--; ++i) {
			size_t r = c32rtomb(NULL, *i, &ps);
			if (r == (size_t)-1) {
				return YIO_ERROR_C32TOMB;
			}
			iout_len += r;
		}
	}
	const size_t out_len = iout_len;

	char * const out = malloc(sizeof(*out) * out_len);
	if (out == NULL) {
		return YIO_ERROR_ENOMEM;
	}

	memset(&ps, 0, sizeof(ps));

	char *iout = out;
	size_t i_len = c32_len;
	for (const char32_t *i = c32; i_len--; ++i) {
		const size_t r = c32rtomb(iout, *i, &ps);
		if (r == (size_t)-1) {
			free(out);
			return YIO_ERROR_C32TOMB;
		}
		iout += r;
	}
	assert(out_len == (size_t)(iout - out));

	*mb = out;
	if (mb_len) {
		*mb_len = out_len;
	}
	return 0;
}

#if _yIO_HAS_WCHAR_H

static inline
int _yIO_strconv_ustr_to_wstr_NE(const char32_t *c32, size_t c32_len, const wchar_t **wc, size_t *wc_len) {
	if (!c32_len) {
		*wc = NULL;
		if (wc_len) {
			*wc_len = 0;
		}
		return 0;
	}

	int ret = 0;

	const char *mb;
	size_t mb_len;
	ret = _yIO_strconv_ustr_to_str(c32, c32_len, &mb, &mb_len);
	if (ret) {
		return ret;
	}

	ret = _yIO_strconv_str_to_wstr(mb, mb_len, wc, wc_len);
	free((void*)mb);
	return ret;
}

static inline
int _yIO_strconv_ustr_to_wstr_EQ(const char32_t *src, size_t src_len, const wchar_t **dst, size_t *dst_len) {
	_yIO_RETURN_STRCONV_SAME(char32_t, wchar_t, src, src_len, dst, dst_len);
}

int _yIO_strconv_ustr_to_wstr(const char32_t *c32, size_t c32_len, const wchar_t **wc, size_t *wc_len) {
#if __STDC_UTF_32__ && __STDC_ISO_10646__
	return _yIO_strconv_ustr_to_wstr_EQ(c32, c32_len, wc, wc_len);
#else
	return _yIO_strconv_ustr_to_wstr_NE(c32, c32_len, wc, wc_len);
#endif
}
#endif // _yIO_HAS_WCHAR_H

»)m4_dnl;
m4_dnl Generate char16_t fucntions by replacing 32 to 16
m4_patsubst(m4_patsubst(m4_uchar_functions(), «32», «16»), «ustr», «c16str»)
m4_dnl Then just generate 32
m4_uchar_functions()
m4_dnl;

int _yIO_strconv_c16str_to_ustr(const char16_t *c16, size_t c16_len, const char32_t **c32, size_t *c32_len) {
#if __STDC_UTF_16__ && __STDC_UTF_32__ && __STDC_ISO_10646__ && _yIO_HAS_UNISTRING
	size_t length;
	uint32_t *result = u16_to_u32(c16, c16_len, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	*c32 = result;
	if (c32_len) {
		*c32_len = length;
	}
	return 0;
#else
#error TODO install libunistring
	return -1;
#endif
}

int _yIO_strconv_ustr_to_c16str(const char32_t *c32, size_t c32_len, const char16_t **c16, size_t *c16_len) {
#if __STDC_UTF_16__ && __STDC_UTF_32__ && __STDC_ISO_10646__ && _yIO_HAS_UNISTRING
	size_t length;
	uint16_t *result = u32_to_u16(c32, c32_len, NULL, &length);
	if (result == NULL) return YIO_ERROR_ENOMEM;
	*c16 = result;
	if (c16_len) {
		*c16_len = length;
	}
	return 0;
#else
#error TODO install libunistring
	return -1;
#endif
}

#endif // _yIO_HAS_UCHAR_H

/* --------------------------------------------------------------------------- */
