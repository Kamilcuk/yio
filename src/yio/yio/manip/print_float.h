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

#ifdef _yIO_HAS_FLOAT$1

int _yIO_print_float_strfrom$1(yio_printctx_t *t);
int _yIO_print_float_stupid$1(yio_printctx_t *t);
int _yIO_print_float_printf$1(yio_printctx_t *t);

static inline
int _yIO_print_float$1(yio_printctx_t *t) {
#ifdef _yIO_HAS_strfrom$1
	return _yIO_print_float_strfrom$1(t);
#else // _yIO_HAS_strfrom$1
#ifdef YIO_FLOATS_PREFER_CUSTOM
	return _yIO_print_float_stupid$1(t);
#else // YIO_FLOATS_PREFER_CUSTOM
	return _yIO_print_float_printf$1(t);
#endif // YIO_FLOATS_PREFER_CUSTOM
#endif // _yIO_HAS_strfrom$1
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

