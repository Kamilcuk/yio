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

/* Types ------------------------------------------------------------------------------------ */

typedef char     _yIO_char_t;
#if _yIO_HAS_WCHAR_H
typedef wchar_t  _yIO_wchar_t;
#endif
#if _yIO_HAS_UCHAR_H
typedef char16_t _yIO_c16char_t;
typedef char32_t _yIO_uchar_t;
#endif

/*  strnlen ------------------------------------------------------------------------------ */

_yIO_wur _yIO_nn()
size_t _yIO_strnlen(const char *str, size_t maxlen);
#if _yIO_HAS_WCHAR_H
_yIO_wur _yIO_nn()
size_t _yIO_wstrnlen(const wchar_t *str, size_t maxlen);
#endif
#if _yIO_HAS_UCHAR_H
_yIO_wur _yIO_nn()
size_t _yIO_c16strnlen(const char16_t *str, size_t maxlen);
_yIO_wur _yIO_nn()
size_t _yIO_ustrnlen(const char32_t *str, size_t maxlen);
#endif

/* Conversion utilities ---------------------------------------------------------------------- */
// I have sinned

#define _yIO_STRCONV_DECLARE_IN2(N1, N2) \
\
_yIO_wur _yIO_access_r(1, 2) _yIO_nn(1, 3) \
int  _yIO_strconv_##N1##str_to_##N2##str(\
		const _yIO_##N1##char_t *src, size_t src_len, \
		const _yIO_##N2##char_t **dst, size_t *dst_len); \
\
static inline _yIO_nn() \
void _yIO_strconv_free_##N1##str_to_##N2##str(\
		const _yIO_##N1##char_t *src, \
		const _yIO_##N2##char_t *dst) { \
	assert((void*)src != dst); \
	free((void*)dst); /* cppcheck-suppress cert-EXP05-C */ \
} \
/* end */
#define _yIO_STRCONV_DECLARE_IN(N1, N2)  _yIO_STRCONV_DECLARE_IN2(N1, N2)
#define _yIO_STRCONV_DECLARE(N1, N2) \
		_yIO_STRCONV_DECLARE_IN(_yIO_STRCONV_ESC N1, _yIO_STRCONV_ESC N2)

#define _yIO_STRCONV_DECLARE_TO_ITSELF_IN2(N1) \
\
static inline _yIO_wur _yIO_access_r(1, 2) _yIO_nn(1, 3) \
int  _yIO_strconv_##N1##str_to_##N1##str(\
		const _yIO_##N1##char_t *src, size_t src_len, \
		const _yIO_##N1##char_t **dst, size_t *dst_len) { \
	*dst = src; \
	if (dst_len) { \
		*dst_len = src_len; \
	} \
	return 0; \
} \
\
static inline _yIO_nn() \
void _yIO_strconv_free_##N1##str_to_##N1##str(\
		const _yIO_##N1##char_t *src, \
		const _yIO_##N1##char_t *dst) { \
	assert(src == dst); \
} \
/* end */

#define _yIO_STRCONV_DECLARE_TO_ITSELF_IN(N) _yIO_STRCONV_DECLARE_TO_ITSELF_IN2(N)
#define _yIO_STRCONV_DECLARE_TO_ITSELF(N) \
		_yIO_STRCONV_DECLARE_TO_ITSELF_IN(_yIO_STRCONV_ESC N)

_yIO_STRCONV_DECLARE_TO_ITSELF(())
#if _yIO_HAS_WCHAR_H
_yIO_STRCONV_DECLARE((), (w))
#endif
#if _yIO_HAS_UCHAR_H
_yIO_STRCONV_DECLARE((), (c16))
_yIO_STRCONV_DECLARE((), (u))
#endif

#if _yIO_HAS_WCHAR_H
_yIO_STRCONV_DECLARE((w), ())
_yIO_STRCONV_DECLARE_TO_ITSELF((w))
#if _yIO_HAS_UCHAR_H
_yIO_STRCONV_DECLARE((w), (c16))
_yIO_STRCONV_DECLARE((w), (u))
#endif
#endif

#if _yIO_HAS_UCHAR_H
_yIO_STRCONV_DECLARE((c16), ())
_yIO_STRCONV_DECLARE((c16), (w))
_yIO_STRCONV_DECLARE_TO_ITSELF((c16))
_yIO_STRCONV_DECLARE((c16), (u))

_yIO_STRCONV_DECLARE((u), ())
_yIO_STRCONV_DECLARE((u), (w))
_yIO_STRCONV_DECLARE((u), (c16))
_yIO_STRCONV_DECLARE_TO_ITSELF((u))
#endif

/* -------------------------------------------------------------- */
#endif /* _yIO_YIO_PRIVATE_YIO_STRLIB_H_ */
