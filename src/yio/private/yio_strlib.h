/**
 * @file
 * @date 2020-lip-31
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_STRLIB_H_
#define _yIO_YIO_PRIVATE_YIO_STRLIB_H_
#include "private.h"
#include <assert.h>
#include <stdlib.h>
#if _yIO_HAS_WCHAR_H
#include <wchar.h>
#endif
#if _yIO_HAS_UCHAR_H
#include <uchar.h>
#endif

// {#
#ifdef _yIO_CDT_PARSER
int _yIO_strconv_str_to_πstr(const char *, size_t, const void *, size_t*);
int _yIO_strconv_wstr_to_πstr(const wchar_t *, size_t, const void *, size_t *);
int _yIO_strconv_c16str_to_πstr(const char16_t *, size_t, const void *, size_t *);
int _yIO_strconv_ustr_to_πstr(const char32_t *, size_t, const void *, size_t *);
int _yIO_strconv_πstr_to_str(const void *, size_t, const char **, size_t*);
int _yIO_strconv_πstr_to_wstr(const void *, size_t, const wchar_t **, size_t *);
int _yIO_strconv_πstr_to_c16str(const void *, size_t, const char16_t **, size_t *);
int _yIO_strconv_πstr_to_ustr(const void *, size_t, const char32_t **, size_t *);
int _yIO_strconv_str_to_str(const char *, size_t, const char **, size_t*);
int _yIO_strconv_wstr_to_str(const wchar_t *, size_t, const char **, size_t *);
int _yIO_strconv_c16str_to_str(const char16_t *, size_t, const char **, size_t *);
int _yIO_strconv_ustr_to_str(const char32_t *, size_t, const char **, size_t *);
int _yIO_strconv_str_to_wstr(const char *, size_t, const wchar_t **, size_t *);
int _yIO_strconv_str_to_c16str(const char *, size_t, const char16_t **, size_t *);
int _yIO_strconv_str_to_ustr(const char *, size_t, const char32_t **, size_t *);
int _yIO_strconv_wstr_to_wstr(const wchar_t *, size_t, const wchar_t **, size_t *);
int _yIO_strconv_wstr_to_c16str(const wchar_t *, size_t, const char16_t **, size_t *);
int _yIO_strconv_wstr_to_ustr(const wchar_t *, size_t, const char32_t **, size_t *);
int _yIO_strconv_c16str_to_c16str(const char16_t *, size_t, const char16_t **, size_t *);
int _yIO_strconv_c16str_to_ustr(const char16_t *, size_t, const char32_t **, size_t *);
int _yIO_strconv_ustr_to_wstr(const char32_t *, size_t, const wchar_t **, size_t *);
int _yIO_strconv_ustr_to_c16str(const char32_t *, size_t, const char16_t **, size_t *);
int _yIO_strconv_ustr_to_ustr(const char32_t *, size_t, const char32_t **, size_t *);
int _yIO_strconv_free_str_to_πstr(const char *, const void *);
int _yIO_strconv_free_wstr_to_πstr(const wchar_t *, const void *);
int _yIO_strconv_free_c16str_to_πstr(const char16_t *, const void *);
int _yIO_strconv_free_ustr_to_πstr(const char32_t *, const void *);
#define _yIO_$1char_t  wchar_t
#define _yIO_$2char_t  wchar_t
#endif
// #}


#define _yIO_STRCONV_ESC(a)  a

/*
 * | Prefix    | meaning                       |
 * | no prefix | multibyte character or string |
 * | 'w'       | wide character or string      |
 * | 'c16'     | char16_t character or string  |
 * | 'u'       | char32_t character or string  |
 */

/* some macros ------------------------------------------------------------------------- */

#if __STDC_UTF_16__ && __STDC_ISO_10646__
#define _yIO_WCHAR_SAME_AS_C16CHAR  1
#define _yIO_C16CHAR_SAME_AS_WCHAR  1
#endif
#if __STDC_UTF_32__ && __STDC_ISO_10646__
#define _yIO_WCHAR_SAME_AS_C32CHAR  1
#define _yIO_C32CHAR_SAME_AS_WCHAR  1
#endif
#if ! defined __STDC_MB_MIGHT_NEQ_WC__
#define _yIO_WCHAR_SAME_AS_CHAR  1
#define _yIO_CHAR_SAME_AS_WCHAR  1
#endif

/*  strnlen ------------------------------------------------------------------------------ */

_yIO_wur _yIO_nn() _yIO_access_r(1, 2)
size_t _yIO_strnlen(const char *str, size_t maxlen);
#if _yIO_HAS_WCHAR_H
_yIO_wur _yIO_nn() _yIO_access_r(1, 2)
size_t _yIO_wstrnlen(const wchar_t *str, size_t maxlen);
#endif
#if _yIO_HAS_UCHAR_H
_yIO_wur _yIO_nn() _yIO_access_r(1, 2)
size_t _yIO_c16strnlen(const char16_t *str, size_t maxlen);
_yIO_wur _yIO_nn() _yIO_access_r(1, 2)
size_t _yIO_ustrnlen(const char32_t *str, size_t maxlen);
#endif

/* Conversion utilities ---------------------------------------------------------------------- */

typedef char     _yIO_char_t;
#if _yIO_HAS_WCHAR_H
typedef wchar_t  _yIO_wchar_t;
#endif
#if _yIO_HAS_UCHAR_H
typedef char16_t _yIO_c16char_t;
typedef char32_t _yIO_uchar_t;
#endif

{% macro _yIO_STRCONV_DECLARE_TO_ITSELF(N1) %}{% call j_APPLY(N1) %}
#line

static inline _yIO_wur _yIO_access_r(1, 2) _yIO_nn(1, 3)
int  _yIO_strconv_$1str_to_$1str(
		const _yIO_$1char_t *in, size_t in_len,
		const _yIO_$1char_t **pout, size_t *pout_len) {
	*pout = in;
	if (pout_len != NULL) {
		*pout_len = in_len;
	}
	return 0;
}

static inline _yIO_nn()
void _yIO_strconv_free_$1str_to_$1str(
		const _yIO_$1char_t *in,
		const _yIO_$1char_t *out) {
	assert(in == out);
}

{% endcall %}{% endmacro %}
{% macro _yIO_STRCONV_DECLARE(N1, N2) %}{% call j_APPLY(N1, N2) %}
#line

_yIO_wur _yIO_access_r(1, 2) _yIO_nn(1, 3) _yIO_access_w(3) _yIO_access_w(4)
int  _yIO_strconv_$1str_to_$2str(
		const _yIO_$1char_t *in, size_t in_len,
		const _yIO_$2char_t **pout, size_t *pout_len);

static inline _yIO_nn()
void _yIO_strconv_free_$1str_to_$2str(
		const _yIO_$1char_t *in,
		const _yIO_$2char_t *out) {
	assert((void *)in != out);
	free((void *)out); /* cppcheck-suppress cert-EXP05-C */
}

{% endcall %}{% endmacro %}

{{ _yIO_STRCONV_DECLARE_TO_ITSELF("") }}
#if _yIO_HAS_WCHAR_H
{{ _yIO_STRCONV_DECLARE("", "w") }}
#endif
#if _yIO_HAS_UCHAR_H
{{ _yIO_STRCONV_DECLARE("", "c16") }}
{{ _yIO_STRCONV_DECLARE("", "u") }}
#endif

#if _yIO_HAS_WCHAR_H
{{ _yIO_STRCONV_DECLARE("w", "") }}
{{ _yIO_STRCONV_DECLARE_TO_ITSELF("w") }}
#if _yIO_HAS_UCHAR_H
{{ _yIO_STRCONV_DECLARE("w", "c16") }}
{{ _yIO_STRCONV_DECLARE("w", "u") }}
#endif
#endif

#if _yIO_HAS_UCHAR_H
{{ _yIO_STRCONV_DECLARE("c16", "") }}
#if _yIO_HAS_WCHAR_H
{{ _yIO_STRCONV_DECLARE("c16", "w") }}
#endif
{{ _yIO_STRCONV_DECLARE_TO_ITSELF("c16") }}
{{ _yIO_STRCONV_DECLARE("c16", "u") }}

{{ _yIO_STRCONV_DECLARE("u", "") }}
#if _yIO_HAS_WCHAR_H
{{ _yIO_STRCONV_DECLARE("u", "w") }}
#endif
{{ _yIO_STRCONV_DECLARE("u", "c16") }}
{{ _yIO_STRCONV_DECLARE_TO_ITSELF("u") }}
#endif

/* -------------------------------------------------------------- */
#endif /* _yIO_YIO_PRIVATE_YIO_STRLIB_H_ */
