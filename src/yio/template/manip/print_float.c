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

#if __CDT_PARSER__
#define _yIO_float_astrfrom_$2$1  _yIO_float_astrfrom_customd
#define _yIO_FLOAT$3  double
#endif

// j_generate_print_floats(type, suffix, promoted_type<for float it's double> )
{% macro j_generate_print_floats() %}{% call(V) j_APPLY(*varargs) %}
#line
{% if V.0 != "f" %}
{% if V.0 == "d" %}
#line
// For double, we export the prototype.
int _yΩIO_print_float_$2$1_in(yπio_printctx_t *t, _yIO_FLOAT$1 var);
{% else %}
#line
// Otherwise, the function is local.
static inline
{% endif %}
#line
int _yΩIO_print_float_$2$1_in(yπio_printctx_t *t, _yIO_FLOAT$1 var) {
	int err = yπio_printctx_init(t);
	if (err) return err;
	const char type = t->pf.type ? t->pf.type : 'g';
	const int precision =  t->pf.precision;
	_yIO_CACHE_AUTO_DECL(_result, result, length);
	err = _yIO_float_astrfrom_$2$1(&result, &length, precision, type, var);
	if (err) return err;
	const bool is_negative = result[0] == '-';
	if (
			isdigit((unsigned char)result[0]) ||
			(is_negative && isdigit((unsigned char)result[1]))
	) {
		err = yπio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
	} else {
		err = yπio_printctx_put(t, result, length);
	}
	_yIO_CACHE_AUTO_FREE(_result, result, length);
	return err;
}

int _yΩIO_print_float_$2$1(yπio_printctx_t *t) {
#line
	// Not float.
	const _yIO_FLOAT$1 var = yπio_printctx_va_arg2(t, _yIO_FLOAT$1, _yIO_FLOAT$1);
	return _yΩIO_print_float_$2$1_in(t, var);
}

{% else %}
#line
int _yΩIO_print_float_$2$1(yπio_printctx_t *t) {
	// Float calls double
	const double var = yπio_printctx_va_arg2(t, _yIO_FLOAT$1, double);
	int _yΩIO_print_float_$2d_in(yπio_printctx_t *t, _yIO_FLOATd var);
	return _yΩIO_print_float_$2d_in(t, var);
}
{% endif %}

#if 0
{% if V.1 == "f" %}
#line
int _yΩIO_print_float_$2$1pnt(yπio_printctx_t *t) {
	const _yIO_FLOAT$1 var = * yπio_printctx_va_arg(t, _yIO_FLOAT$1 *);
	return _yΩIO_print_float_$2$1_in(t, var);
}
{% endif %}
#endif

{% endcall %}{% endmacro %}

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

{{ j_generate_print_floats(V.0, "strfrom") }}

#if _yIO_has_float_custom$1
{{ j_generate_print_floats(V.0, "custom") }}
#endif

#if _yIO_has_float_printf$1
{{ j_generate_print_floats(V.0, "printf") }}
#endif

#if _yIO_has_float_ryu$1
{{ j_generate_print_floats(V.0, "ryu") }}
#endif

#endif

{% endcall %}
