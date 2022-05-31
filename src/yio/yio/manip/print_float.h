/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_FLOAT_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_FLOAT_H_
#include "../../private/yio_float.h"
#include "../../private/yio_float_strfrom_custom.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_printf.h"
#include "../../private/yio_float_strfrom_ryu.h"
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
#ifndef YIO_PRINT_FLOATS_WITH_RYU
#define YIO_PRINT_FLOATS_WITH_RYU      4
#endif

#ifndef YIO_PRINT_FLOATS_WITH
#error YIO_PRINT_FLOATS_WITH has to be configured
#endif


{% call j_FOREACHAPPLY(j_FLOATS) %}
#line

#ifndef YYIO_HAS_FLOAT$1
#error  YYIO_HAS_FLOAT$1
#endif
#if YYIO_HAS_FLOAT$1

#ifndef YYIO_HAS_strfrom$1
#error  YYIO_HAS_strfrom$1
#endif
#ifndef YYIO_has_float_custom$1
#error  YYIO_has_float_custom$1
#endif
#ifndef YYIO_has_float_printf$1
#error  YYIO_has_float_printf$1
#endif
#ifndef YYIO_has_float_ryu$1
#error  YYIO_has_float_ryu$1
#endif

int YYΩIO_print_float_strfrom$1(yπio_printctx_t *t);
#if YYIO_has_float_custom$1
int YYΩIO_print_float_custom$1(yπio_printctx_t *t);
#endif
#if YYIO_has_float_printf$1
int YYΩIO_print_float_printf$1(yπio_printctx_t *t);
#endif
#if YYIO_has_float_ryu$1
int YYΩIO_print_float_ryu$1(yπio_printctx_t *t);
#endif

#ifndef YYΩIO_PRINT_FLOAT$1
#	if YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_STRFROM
#		define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_strfrom$1
#	elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_CUSTOM && YYIO_has_float_custom$1
#		define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_custom$1
#	elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_PRINTF && YYIO_has_float_printf$1
#		define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_printf$1
#	elif YIO_PRINT_FLOATS_WITH == YIO_PRINT_FLOATS_WITH_RYU && YYIO_has_float_ryu$1
#		define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_ryu$1
#	else
#		if YYIO_HAS_strfrom$1
#			define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_strfrom$1
#		elif YYIO_has_float_custom$1
#			define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_custom$1
#		else
#			define YYΩIO_PRINT_FLOAT$1  YYΩIO_print_float_strfrom$1
#		endif
#	endif
#endif

#define YYΩIO_PRINT_FUNC_GENERIC_FLOAT$1()  \
		YYIO_FLOAT$1: YYΩIO_PRINT_FLOAT$1,

#else

#define YYΩIO_PRINT_FUNC_GENERIC_FLOAT$1()

#endif

{% endcall %}

#define YYΩIO_PRINT_FUNC_GENERIC_FLOATS() \
		{% call j_FOREACHAPPLY(j_FLOATS) %}
		YYΩIO_PRINT_FUNC_GENERIC_FLOAT$1() \
		{% endcall %}
		/**/

#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_FLOAT_H_ */
