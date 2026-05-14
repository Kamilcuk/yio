/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#define YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifndef YIO_PRIVATE
#error THIS FILE IS ONLY FOR PRIVATE USE
#endif

#include "../yio_config.h"
#include "yio_string.h"
#include "yio_float.h"

{% call(V) j_FOREACHAPPLY(j_FLOATREPRS) %}
#line
#ifdef YIO_FLOAT_RP_$1

#if YIO_USE_STRFROM_PRINTF
#  ifdef YIO_FLOAT_PRI_RP_$1
#    define YIO_has_float_astrfrom_printf_$1  1
/**
 * Convert the floating number val according to specified precision
 * and specification using snprintf function.
 * @param v
 * @param precision0 Negative if unset, non-negative represents exact precision.
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YIO_float_astrfrom_printf_$1(YIO_string *v, int precision0, char spec, YIO_FLOAT_RP_$1 val);
#  else
#    define YIO_has_float_astrfrom_printf_$1  0
#  endif
#else
#  define YIO_has_float_astrfrom_printf_$1  0
#endif

#else
#define YIO_has_float_astrfrom_printf_$1 0
#endif
{% endcall %}

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_PRINTF_H_
