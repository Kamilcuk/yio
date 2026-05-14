/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_NAIVE_H_
#define YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_NAIVE_H_
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

/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param v @see YIO_string
 * @param precision0 Negative if unset, non-negative represents exact precision.
 * @param spec0 a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YIO_float_astrfrom_naive_$1(YIO_string *v, int precision0, char spec0, YIO_FLOAT_RP_$1 val);

#define YIO_has_float_astrfrom_naive_$1  1
#else
#define YIO_has_float_astrfrom_naive_$1  0
#endif
{% endcall %}

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_NAIVE_H_
