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
{% macro j_generate_print_floats() %}{% call(V) j_APPLY(*varargs) %}
#line
{% if V.0 != "f" %}
{% if V.0 == "d" %}
#line
// For double, we export the prototype.
int YYΩIO_print_float_$2$1_in(yπio_printctx_t *t, YYIO_FLOAT$1 var);
{% else %}
#line
// Otherwise, the function is local.
static inline
{% endif %}
#line
int YYΩIO_print_float_$2$1_in(yπio_printctx_t *t, YYIO_FLOAT$1 var) {
	int err = yπio_printctx_init(t);
	if (err) return err;
	// TODO: fun fact, this converts from wchar/char16/char32 -> char. Convert for real with some checks here.
	const char type = (char)(t->pf.type ? t->pf.type : 'g');
	const int precision =  t->pf.precision;
	YYIO_RES_AUTO_DECL(res);
	err = YYIO_float_astrfrom_$2$1(&res, precision, type, var);
	if (err) return err;
	const char *const result = YYIO_res_begin(&res);
	const size_t length = YYIO_res_used(&res);
	const bool is_negative = result[0] == '-';
	//if (
			//isdigit((unsigned char)result[0]) ||
			//(is_negative && isdigit((unsigned char)result[1]))
	//) {
		err = yπio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
	//} else {
		//err = yπio_printctx_put(t, result, length);
	//}
	YYIO_res_end(&res);
	return err;
}

int YYΩIO_print_float_$2$1(yπio_printctx_t *t) {
#line
	// Not float.
	const YYIO_FLOAT$1 var = yπio_printctx_va_arg_promote(t, YYIO_FLOAT$1);
	return YYΩIO_print_float_$2$1_in(t, var);
}

{% else %}
#line
int YYΩIO_print_float_$2$1(yπio_printctx_t *t) {
	// Float calls double
	const double var = yπio_printctx_va_arg_promote(t, YYIO_FLOAT$1);
	int YYΩIO_print_float_$2d_in(yπio_printctx_t *t, YYIO_FLOATd var);
	return YYΩIO_print_float_$2d_in(t, var);
}
{% endif %}

#if 0
{% if V.1 == "f" %}
#line
int YYΩIO_print_float_$2$1pnt(yπio_printctx_t *t) {
	const YYIO_FLOAT$1 var = * yπio_printctx_va_arg(t, YYIO_FLOAT$1 *);
	return YYΩIO_print_float_$2$1_in(t, var);
}
{% endif %}
#endif

{% endcall %}{% endmacro %}

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

{{ j_generate_print_floats(V.0, "strfrom") }}

#if YYIO_has_float_custom$1
{{ j_generate_print_floats(V.0, "custom") }}
#endif

#if YYIO_has_float_printf$1
{{ j_generate_print_floats(V.0, "printf") }}
#endif

#if YYIO_has_float_ryu$1
{{ j_generate_print_floats(V.0, "ryu") }}
#endif

#endif

{% endcall %}
