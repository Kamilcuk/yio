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

#include "../yio_config.h"
#include "yio_float.h"
#ifdef YYIO_PRIVATE
#include "yio_string.h"
#endif

{% call(V) j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1
{% if V.1 == "f" or V.1 == "d" %}
#ifdef YYIO_PRIVATE
/**
 * Convert the floating number val according to specified precision
 * and specification using method written by myself.
 * @param res
 * @param precision Negative if unset
 * @param spec a, A, e, E, f, F, g, or G
 * @param val The floating point value to convert
 * @return 0 on success, error otherwise
 */
int YYIO_float_astrfrom_ryu$1(YYIO_string *res, int precision, char spec0, YYIO_FLOAT$1 val);
#endif
#define YYIO_has_float_ryu$1  1
{% elif V.1 == "l" %}
#if YYIO_HAS_INT128
#ifdef YYIO_PRIVATE
/** long double native support requires __int128. */
int YYIO_float_astrfrom_ryu$1(YYIO_string *res, int precision, char spec0, YYIO_FLOAT$1 val);
#endif
#define YYIO_has_float_ryu$1  1
#elif YYIO_FLOAT_MANT_DIG$1 == FLT_MANT_DIG
#define YYIO_has_float_ryu$1  1
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1(res, prec, spec, val)  YYIO_float_astrfrom_ryuf(res, prec, spec, (float)(val))
#endif
#elif YYIO_FLOAT_MANT_DIG$1 == DBL_MANT_DIG
#define YYIO_has_float_ryu$1  1
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1(res, prec, spec, val)  YYIO_float_astrfrom_ryud(res, prec, spec, (double)(val))
#endif
#else
#define YYIO_has_float_ryu$1  0
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1  YYIO_float_astrfrom_custom$1
#endif
#endif
{% elif j_search("^f[0-9]+", V.1) %}
#if YYIO_FLOAT_MANT_DIG$1 == FLT_MANT_DIG
#define YYIO_has_float_ryu$1  1
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1(res, prec, spec, val)  YYIO_float_astrfrom_ryuf(res, prec, spec, (float)(val))
#endif
#elif YYIO_FLOAT_MANT_DIG$1 == DBL_MANT_DIG
#define YYIO_has_float_ryu$1  1
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1(res, prec, spec, val)  YYIO_float_astrfrom_ryud(res, prec, spec, (double)(val))
#endif
#else
#define YYIO_has_float_ryu$1  0
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1  YYIO_float_astrfrom_custom$1
#endif
#endif
{% else %}
#define YYIO_has_float_ryu$1  0
#ifdef YYIO_PRIVATE
#define YYIO_float_astrfrom_ryu$1  YYIO_float_astrfrom_custom$1
#endif
{% endif %}
#else
#define YYIO_has_float_ryu$1  0
#endif // YYIO_FLOAT$1
{% endcall %}

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_FLOAT_STRFROM_RYU_H_
