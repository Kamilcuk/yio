/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#include "../yio_config.h"
#ifdef YYIO_PRIVATE
#include "yio_res.h"
#endif

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
{% if V.0 in ["f", "d", "l", "d32", "d64", "d128"] %}
#if YIO_HAS_FLOAT$1

#ifdef YYIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using snprintf function.
 * @param o
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_printf$1(YYIO_res *v, int precision, char spec, YYIO_FLOAT$1 val);
#endif

#define YYIO_has_float_printf$1  1
#else
#define YYIO_has_float_printf$1  0
#endif
{% else %}
#define YYIO_has_float_printf$1 0
{% endif %}
{% endcall %}

#endif /* YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_ */
