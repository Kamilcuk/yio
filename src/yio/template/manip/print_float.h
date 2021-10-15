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
#include "../../private/yio_float_strfrom_custom.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_printf.h"
#include "../ctx.h"

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
#error YIO_PRINT_FLOATS_WITH has to be configured
#endif


m4_ifdef(«m4_floatlist», «», 
«m4_define(«m4_floatlist», «
	(f), (d), (l),
	(f16), (f32), (f64), (f128),
	(f32x), (f64x), (f128x),
	(d32), (d64), (d128),
»)
»)

m4_applysync((m4_floatlist), «)

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

#ifndef _yIO_HAS_strfrom$1
#error  _yIO_HAS_strfrom$1
#endif
#ifndef _yIO_has_float_custom$1
#error  _yIO_has_float_custom$1
#endif
#ifndef _yIO_has_float_printf$1
#error  _yIO_has_float_printf$1
#endif

int _yΩIO_print_float_strfrom$1(yπio_printctx_t *t);
#if _yIO_has_float_custom$1
int _yΩIO_print_float_custom$1(yπio_printctx_t *t);
#endif
#if _yIO_has_float_printf$1
int _yΩIO_print_float_printf$1(yπio_printctx_t *t);
#endif

#ifndef _yΩIO_PRINT_FLOAT$1
#	if YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_STRFROM
#		define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_strfrom$1
#	elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_CUSTOM && _yIO_has_float_custom$1
#		define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_custom$1
#	elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_PRINTF && _yIO_has_float_printf$1
#		define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_printf$1
#	else
#		if _yIO_HAS_strfrom$1
#			define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_strfrom$1
#		elif _yIO_has_float_custom$1
#			define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_custom$1
#		else
#			define _yΩIO_PRINT_FLOAT$1  _yΩIO_print_float_strfrom$1
#		endif
#	endif
#endif

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		_yIO_FLOAT$1: _yΩIO_PRINT_FLOAT$1,

#else

#define _yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif

»)

#define _yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
m4_applyforeachdefine((m4_floatlist),
		«_yΩIO_PRINT_FUNC_GENERIC_FLOAT$1()»)

