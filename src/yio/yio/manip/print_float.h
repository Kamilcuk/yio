/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
#define YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../../private/yio_float.h"
#include "../../private/yio_float_strfrom_naive.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_printf.h"
#include "../../private/yio_float_strfrom_ryu.h"
#include "../ctx.h"

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1  is not defined
#endif
#if YIO_HAS_FLOAT$1

#ifndef YYIO_HAS_strfrom$1
#error  YYIO_HAS_strfrom$1 is not defined
#endif
#ifndef YYIO_has_float_naive$1
#error  YYIO_has_float_naive$1 is not defined
#endif
#ifndef YYIO_has_float_printf$1
#error  YYIO_has_float_printf$1 is not defined
#endif
#ifndef YYIO_has_float_ryu$1
#error  YYIO_has_float_ryu$1 is not defined
#endif

#if YYIO_has_float_strfrom$1
int YYIO_print_float_strfrom$1(yio_printctx_t *t);
#endif
#if YYIO_has_float_naive$1
int YYIO_print_float_naive$1(yio_printctx_t *t);
#endif
#if YYIO_has_float_printf$1
int YYIO_print_float_printf$1(yio_printctx_t *t);
#endif
#if YYIO_has_float_ryu$1
int YYIO_print_float_ryu$1(yio_printctx_t *t);
#endif

#ifndef YYIO_PRINT_FLOAT$1
#	if YIO_FLOAT_BACKEND_STRFROM
#		define YYIO_PRINT_FLOAT$1  YYIO_print_float_strfrom$1
#	elif YIO_FLOAT_BACKEND_NAIVE && YYIO_has_float_naive$1
#		define YYIO_PRINT_FLOAT$1  YYIO_print_float_naive$1
#	elif YIO_FLOAT_BACKEND_PRINTF && YYIO_has_float_printf$1
#		define YYIO_PRINT_FLOAT$1  YYIO_print_float_printf$1
#	elif YIO_FLOAT_BACKEND_RYU && YYIO_has_float_ryu$1
#		define YYIO_PRINT_FLOAT$1  YYIO_print_float_ryu$1
#	else
#		if YYIO_HAS_strfrom$1
#			define YYIO_PRINT_FLOAT$1  YYIO_print_float_strfrom$1
#		elif YYIO_has_float_naive$1
#			define YYIO_PRINT_FLOAT$1  YYIO_print_float_naive$1
#		else
#			define YYIO_PRINT_FLOAT$1  YYIO_print_float_strfrom$1
#		endif
#	endif
#endif


#ifdef __cplusplus
{% if V.G == "s" %}
#define YYIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		YYIO_OVERLOAD_TYPE_FUNC(YYIO_FLOAT$1, YYIO_PRINT_FLOAT$1)
{% else %}
#define YYIO_PRINT_FUNC_GENERIC_FLOAT$1()
{% endif %}
#else
#define YYIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		YYIO_OVERLOAD_TYPE_FUNC(YYIO_FLOAT$1, YYIO_PRINT_FLOAT$1)
#endif

#else

#define YYIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif

{% endcall %}

#define YYIO_PRINT_FUNC_GENERIC_FLOATS() \
		{% call j_FOREACHAPPLY(j_FLOATS) %}
		YYIO_PRINT_FUNC_GENERIC_FLOAT$1() \
		{% endcall %}
		/**/

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_FLOAT_H_
