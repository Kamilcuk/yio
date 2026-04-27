/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_CUSTOM_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_CUSTOM_H_
#include "../yio_config.h"
#ifdef YYIO_PRIVATE
#include "yio_string.h"
#endif

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
{% if V.1 in [
	"f", "d", "l",
	"f16", "f32", "f64", "f128", "f32x", "f64x", "f128x",
] %}
#line
#if YIO_HAS_FLOAT$1

#ifdef YYIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param v @see YYIO_string
 * @param precision0 Negative if unset
 * @param spec0 a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_custom$1(YYIO_string *v, int precision0, char spec0, YYIO_FLOAT$1 val);
#endif

#define YYIO_has_float_custom$1  1
#else
#define YYIO_has_float_custom$1  0
#endif // YYIO_FLOAT$1
{% else %}
#line
#define YYIO_has_float_custom$1  0
{% endif %}
{% endcall %}

#endif /* YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_CUSTOM_H_ */
