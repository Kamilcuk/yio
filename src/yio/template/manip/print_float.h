/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../../private/yio_float.h"

m4_applyforeachdefine(`(
		(f, float),
		(fpnt, float *),
		(, double),
		(l, long double)
)~,`m4_dnl;

#ifndef YIO_PRINT_FLOATS_WITH_UNSET
#define YIO_PRINT_FLOATS_WITH_UNSET  0
#endif
#ifndef YIO_PRINT_FLOATS_WITH_STRFROM
#define YIO_PRINT_FLOATS_WITH_STRFROM  1
#endif
#ifndef YIO_PRINT_FLOATS_WITH_PRINTF
#define YIO_PRINT_FLOATS_WITH_PRINTF   2
#endif
#ifndef YIO_PRINT_FLOATS_WITH_CUSTOM
#define YIO_PRINT_FLOATS_WITH_CUSTOM   3
#endif

#ifdef _yIO_HAS_FLOAT$1

#ifdef _yIO_HAS_strfrom$1
int _yΩIO_print_float_strfrom$1(yπio_printctx_t *t);
#endif
int _yΩIO_print_float_stupid$1(yπio_printctx_t *t);
int _yΩIO_print_float_printf$1(yπio_printctx_t *t);

#ifdef YIO_PRINT_FLOATS_WITH
#if YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_STRFROM
#ifndef _yIO_HAS_strfrom$1
#error "Requested printing floats with strfrom, but it's not supported"
#endif
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_strfrom$1
#elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_PRINTF
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_printf$1
#elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_CUSTOM
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_stupid$1
#else
#error "Invalid value for YIO_PRINT_FLOATS_WITH. It must be set to one of YIO_PRINT_FLOATS_WITH_* macros"
#endif
#else // YIO_PRINT_FLOATS_WITH
	// If no printing function was specified, use defaults
#ifdef _yIO_HAS_strfrom$1
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_strfrom$1
#else // _yIO_HAS_strfrom$1
#ifdef YIO_FLOATS_PREFER_CUSTOM
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_stupid$1
#else // YIO_FLOATS_PREFER_CUSTOM
#define _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC _yΩIO_print_float_printf$1
#endif // YIO_FLOATS_PREFER_CUSTOM
#endif // _yIO_HAS_strfrom$1
#endif // YIO_PRINT_FLOATS_WITH

#ifndef _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC
#error
#endif

static inline
int (*_yΩIO_print_float$1_printing_func(void))(yπio_printctx_t *t) {
	return _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC;
}

static inline
int _yΩIO_print_float$1(yπio_printctx_t *t) {
	return _ΩIO_PRINT_FLOAT$1_PRINTING_FUNC(t);
}

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		$2: _yΩIO_print_float$1,

#else  // _yIO_HAS_FLOAT$1

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif // _yIO_HAS_FLOAT$1

~)m4_dnl;

#define _yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
m4_applyforeachdefine(`(
		(f),
		(fpnt),
		(),
		(l)
)~, `_yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()~, `~)

