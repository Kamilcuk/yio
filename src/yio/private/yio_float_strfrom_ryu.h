/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifndef YYIO_PRIVATE
#error THIS FILE IS ONLY FOR PRIVATE USE
#endif

#include "../yio_config.h"

#  ifndef YYIO_HAS_INT128
#    error  YYIO_HAS_INT128
#  endif

#include "yio_string.h"
#include "yio_float.h"

{% call(V) j_FOREACHAPPLY(j_FLOATREPRS) %}
#line
#ifdef YYIO_FLOAT_RP_$1

{% if V.1 in ["B32", "B64"] %}
#  define YYIO_has_float_astrfrom_ryu_$1 1
{% elif V.1 in ["B80", "B128"] %}
#  if YYIO_HAS_INT128
#    define YYIO_has_float_astrfrom_ryu_$1 1
#  else
#    define YYIO_has_float_astrfrom_ryu_$1 0
#  endif
{% else %}
#  define YYIO_has_float_astrfrom_ryu_$1 0
{% endif %}

/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param res
 * @param precision0 Negative if unset, non-negative represents exact precision.
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_ryu_$1(YYIO_string *res, int precision, char spec0, YYIO_FLOAT_RP_$1 val);

#else
#define YYIO_has_float_astrfrom_ryu_$1 0
#endif
{% endcall %}

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
