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
		(f),
		(fpnt),
		(),
		(l)
)~,`m4_dnl;
int _yIO_print_float_strfrom$1(yio_printctx_t *t);
int _yIO_print_float_stupid$1(yio_printctx_t *t);
int _yIO_print_float_printf$1(yio_printctx_t *t);
~)m4_dnl;

m4_applyforeachdefine(`(
		(f),
		(fpnt),
		(),
		(l)
)~,`m4_dnl;

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

~)m4_dnl;

#define _yIO_PRINT_FUNC_GENERIC_FLOATS() \
		m4_dnl;
m4_applyforeachdefine(`(
		(float, f),
		(float *, fpnt),
		(double, ),
		(long double, l)
)~,
		`$1: _yIO_print_float$2,~m4_dnl;
)m4_dnl;

