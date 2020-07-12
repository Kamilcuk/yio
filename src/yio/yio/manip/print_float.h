/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config(yio);

m4_applyforeachdefine(`(
		(f, f, float),
		(fpnt, f, float *),
		(, , double),
		(l, l, long double)
)~,`m4_dnl;

#ifdef _yIO_HAS_FLOAT$2

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
		$3: _yIO_print_float$1,

#else  // _yIO_HAS_FLOAT$2

#define _yIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif // _yIO_HAS_FLOAT$2

~)m4_dnl;

#define _yIO_PRINT_FUNC_GENERIC_FLOATS() \
m4_applyforeachdefine(`(
		(f),
		(fpnt),
		(),
		(l)
)~, `_yIO_PRINT_FUNC_GENERIC_FLOAT$1()~, `~)

