/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio() m4_dnl;
#pragma once
#include "../../private/yio_float.h"

m4_applyforeachdefine(`(
		(f, float),
		(fpnt, float *),
		(, double),
		(l, long double)
)~,`m4_dnl;

#define YIO_PRINT_FLOATS_WITH_STRFROM  1
#define YIO_PRINT_FLOATS_WITH_PRINTF   2
#define YIO_PRINT_FLOATS_WITH_CUSTOM   3

#ifdef _yIO_HAS_FLOAT$1

int _yIO_print_float_strfrom$1(yio_printctx_t *t);
int _yIO_print_float_stupid$1(yio_printctx_t *t);
int _yIO_print_float_printf$1(yio_printctx_t *t);

static inline
int _yIO_print_float$1(yio_printctx_t *t) {
#ifdef YIO_PRINT_FLOATS_WITH
#if YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_STRFROM
	return _yIO_print_float_strfrom$1(t);
#elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_PRINTF
	return _yIO_print_float_printf$1(t);
#elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_CUSTOM
	return _yIO_print_float_stupid$1(t);
#else
#error "Invalid value for YIO_PRINT_FLOATS_WITH. It must be set to one of YIO_PRINT_FLOATS_WITH_* macros"
#endif
#else // YIO_PRINT_FLOATS_WITH
	// If no printing function was specified, use defaults
#ifdef _yIO_HAS_strfrom$1
	return _yIO_print_float_strfrom$1(t);
#else // _yIO_HAS_strfrom$1
#ifdef YIO_FLOATS_PREFER_CUSTOM
	return _yIO_print_float_stupid$1(t);
#else // YIO_FLOATS_PREFER_CUSTOM
	return _yIO_print_float_printf$1(t);
#endif // YIO_FLOATS_PREFER_CUSTOM
#endif // _yIO_HAS_strfrom$1
#endif // YIO_PRINT_FLOATS_WITH
}

#define _yIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		$2: _yIO_print_float$1,

#else  // _yIO_HAS_FLOAT$1

#define _yIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif // _yIO_HAS_FLOAT$1

~)m4_dnl;

#define _yIO_PRINT_FUNC_GENERIC_FLOATS() \
m4_applyforeachdefine(`(
		(f),
		(fpnt),
		(),
		(l)
)~, `_yIO_PRINT_FUNC_GENERIC_FLOAT$1()~, `~)

