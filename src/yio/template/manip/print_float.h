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
#include "../../private/yio_float_strfrom_stupid.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_printf.h"

#ifndef YIO_PRINT_FLOATS_WITH_STRFROM
#define YIO_PRINT_FLOATS_WITH_STRFROM  1
#endif
#ifndef YIO_PRINT_FLOATS_WITH_CUSTOM
#define YIO_PRINT_FLOATS_WITH_CUSTOM   2
#endif
#ifndef YIO_PRINT_FLOATS_WITH_PRINTF
#define YIO_PRINT_FLOATS_WITH_PRINTF   3
#endif

#ifndef YIO_PRINT_FLOATS_WITH
#error YIO_PRINT_FLOATS_WITH
#endif

m4_applyforeachdefine(«(
	(f,    float,       f),
	(fpnt, float *,     f),
	(d,    double,      d),
	(l,    long double, l),
	(d32,  _Decimal32,  d32),
	(d64,  _Decimal64,  d64),
	(d128, _Decimal128, d128),
)», m4_syncline(1)«m4_dnl;

#ifndef _yIO_HAS_FLOAT$3
#error _yIO_HAS_FLOAT$3
#endif
#ifndef _yIO_HAS_strfrom$3
#error _yIO_HAS_strfrom$3
#endif
#ifndef _yIO_has_float_stupid$3
#error _yIO_has_float_stupid$3
#endif

#if _yIO_HAS_FLOAT$3

int _yΩIO_print_float_strfrom$1(yπio_printctx_t *t);
#if _yIO_has_float_stupid$3
int _yΩIO_print_float_stupid$1(yπio_printctx_t *t);
#endif
int _yΩIO_print_float_printf$1(yπio_printctx_t *t);

#ifndef _yΩIO_PRINT_FLOAT$1
#if YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_CUSTOM && _yIO_has_float_stupid$3
#define _yΩIO_PRINT_FLOAT$1 _yΩIO_print_float_stupid$1
#elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_PRINTF
#define _yΩIO_PRINT_FLOAT$1 _yΩIO_print_float_printf$1
#else
#ifdef __NEWLIB__
// Newlib as of now has no strfrom* function.
#define _yΩIO_PRINT_FLOAT$1 _yΩIO_print_float_stupid$1
#else
#define _yΩIO_PRINT_FLOAT$1 _yΩIO_print_float_strfrom$1
#endif
#endif
#endif

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		$2: _yΩIO_PRINT_FLOAT$1,

#else  // _yIO_HAS_FLOAT$3

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif // _yIO_HAS_FLOAT$3

»)m4_dnl;

#define _yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
m4_applyforeachdefine(«(
		(f),
		(fpnt),
		(d),
		(l),
		(d32),
		(d64),
		(d128),
)», «_yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()», «»)

