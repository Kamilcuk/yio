/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_float.h"
#include <ctype.h>

#if YYIO_CDT_PARSER
#define YYIO_float_astrfrom_$2$1  YYIO_float_astrfrom_customd
#define YYIO_FLOAT$3  double
#endif

// j_generate_print_floats(type, suffix, promoted_type<for float it's double> )
{% macro j_generate_print_floats() %}{% call(V) j_APPLY(*varargs) %} #line
{% if V.1 == "f" %} #line

// Forward declaration for double type printer.
static inline int YYIO_print_float_$2d_in(yio_printctx_t *t, double var);
#if 0
int YYIO_print_float_$2$1pnt(yio_printctx_t *t) {
	const YYIO_FLOAT$1 var = * yio_printctx_va_arg(t, YYIO_FLOAT$1 *);
	return YYIO_print_float_$2$1_in(t, var);
}
#endif
int YYIO_print_float_$2$1(yio_printctx_t *t) {
	// Float calls double
	const double var = yio_printctx_va_arg(t, double);
	return YYIO_print_float_$2d_in(t, var);
}

{% else %} #line

static inline int YYIO_print_float_$2$1_in(yio_printctx_t *t, YYIO_FLOAT$1 var) {
	int err = yio_printctx_init(t);
	if (err) return err;
	const char type = t->pf.type ? t->pf.type : 'g';
	const int precision =  t->pf.precision;
	YYIO_string res;
	YYIO_string_init(&res);
	err = YYIO_float_astrfrom_$2$1(&res, precision, type, var);
	if (err) return err;
	const char *const result = YYIO_string_data(&res);
	const size_t length = YYIO_string_len(&res);
	const bool is_negative = result[0] == '-';
	//if (
			//isdigit((unsigned char)result[0]) ||
			//(is_negative && isdigit((unsigned char)result[1]))
	//) {
		err = yio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
	//} else {
		//err = yio_printctx_put(t, result, length);
	//}
	YYIO_string_free(&res);
	return err;
}

int YYIO_print_float_$2$1(yio_printctx_t *t) {
	const YYIO_FLOAT$1 var = yio_printctx_va_arg_promote(t, YYIO_FLOAT$1);
	return YYIO_print_float_$2$1_in(t, var);
}

{% endif %} #line
{% endcall %}{% endmacro %} #line

{% call(V) j_FOREACHAPPLY(j_FLOATS) %} #line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

#if YYIO_has_float_strfrom$1
{{ j_generate_print_floats(V.1, "strfrom") }}
#endif

#if YYIO_has_float_custom$1
{{ j_generate_print_floats(V.1, "custom") }}
#endif

#if YYIO_has_float_printf$1
{{ j_generate_print_floats(V.1, "printf") }}
#endif

#if YYIO_has_float_ryu$1
{{ j_generate_print_floats(V.1, "ryu") }}
#endif

#endif

{% endcall %}
