// NOLINE
/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief Floating point type definitions.
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include <float.h>
#include <math.h>

/* ------------------------------------------------------------------------- */

{% macro j_robust_define(macro_name, standard_name, default="") %}
#if defined(__{{ standard_name }}__)
#define {{ macro_name }}  __{{ standard_name }}__
#elif defined({{ standard_name }})
#define {{ macro_name }}  {{ standard_name }}
{% if default != "" %}
#else
#define {{ macro_name }}  {{ default }}
{% else %}
#else
#error Can not define {{ macro_name }}
{% endif %}
#endif
{% endmacro %}

/* ------------------------------------------------------------------------- */

{% call(v) j_FOREACHAPPLY(j_FLOATS) %}
{% call j_APPLY(v.N, v.math, v.CONSTPREF) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1
#define YYIO_FLOAT_C$1(x)  (x{{ " ## " + v.CS_VAL if v.CS_VAL else "" }})
#define YYIO_FLOAT_PRI$1   {{v.PRI_VAL}}
{{ j_robust_define("YYIO_FLOAT_MANT_DIG$1", v.CONSTPREF + "_MANT_DIG", v.MANT_VAL) }}
#ifdef INFINITY
{{ j_robust_define("YYIO_FLOAT_HUGE_VAL$1", "HUGE_VAL" + v.HUGE_VAL_SUFF, "((YYIO_FLOAT$1)INFINITY)") }}
#else
{{ j_robust_define("YYIO_FLOAT_HUGE_VAL$1", "HUGE_VAL" + v.HUGE_VAL_SUFF) }}
#endif
{{ j_robust_define("YYIO_FLOAT_MAX$1", "$3_MAX") }}
{{ j_robust_define("YYIO_FLOAT_EPSILON$1", "$3_EPSILON") }}
{{ j_robust_define("YYIO_FLOAT_MIN$1", "$3_MIN") }}
{{ j_robust_define("YYIO_MAX_10_EXP$1", "$3_MAX_10_EXP", v.EXP10_VAL) }}


/**
 * Like frexp(), but always with base 2.
 */
{% if v.G.startswith('d') %}
YYIO_FLOAT$1 YYIO_frexp2$1(YYIO_FLOAT$1 x, int *exp);
{% else %}
#define YYIO_frexp2$1  frexp$2
{% endif %}

#ifndef frexp$2
YYIO_FLOAT$1 frexp$2(YYIO_FLOAT$1 x, int *exp);
#endif
#define YYIO_frexp$1     frexp$2

/// Like frexp(), but always with base 10.
YYIO_FLOAT$1 YYIO_frexp10$1(YYIO_FLOAT$1 x, int *exp);

#ifndef floor$2
YYIO_FLOAT$1 floor$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_floor$1   floor$2

#ifndef exp2$2
YYIO_FLOAT$1 exp2$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_exp2$1    exp2$2

#ifndef log2$2
YYIO_FLOAT$1 log2$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_log2$1    log2$2

#ifndef log10$2
YYIO_FLOAT$1 log10$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_log10$1   log10$2

#ifndef fabs$2
YYIO_FLOAT$1 fabs$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_fabs$1    fabs$2

#ifndef pow$2
YYIO_FLOAT$1 pow$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#endif
#define YYIO_pow$1     pow$2

#ifndef nextafter$2
YYIO_FLOAT$1 nextafter$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#endif
#define YYIO_nextafter$1  nextafter$2

#ifndef modf$2
YYIO_FLOAT$1 modf$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 *iptr);
#endif
#define YYIO_modf$1       modf$2

#ifndef YYIO_HAS_exp10$1
#error YYIO_HAS_exp10$1 is not defined
#endif
#if YYIO_HAS_exp10$1
#ifndef exp10$2
YYIO_FLOAT$1 exp10$2(YYIO_FLOAT$1 x);
#endif
#define YYIO_exp10$1  exp10$2
#else
static inline
YYIO_FLOAT$1 YYIO_exp10$1(YYIO_FLOAT$1 x) {
	return pow$2(YYIO_FLOAT_C$1(10.0), x);
}
#endif

#define YYIO_strto$1  strto{{v.strto}}

#endif
{% endcall %}
{% endcall %}

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}
#if YIO_HAS_FLOAT{{V.1}}
{% if j_search(V.1, "^d[0-9]") %}
/* Decimal Representations */
{% for R in j_FLOATREPRS if R.name.startswith('D') %}
{% if loop.first %}#  if{% else %}#  elif{% endif %} YYIO_FLOAT_MANT_DIG{{V.1}} == {{R.mant}}
#    define YYIO_RP_OF_{{V.1}}_IS_{{R.name}} 1
#    ifndef YYIO_FLOAT_RP_{{R.name}}
#      define YYIO_FLOAT_RP_{{R.name}} YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RP_{{R.name}} YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RP_{{R.name}}   YYIO_floor{{V.1}}
#      define YYIO_modf_RP_{{R.name}}    YYIO_modf{{V.1}}
#      define YYIO_log2_RP_{{R.name}}    YYIO_log2{{V.1}}
#      define YYIO_log10_RP_{{R.name}}   YYIO_log10{{V.1}}
#      define YYIO_exp2_RP_{{R.name}}    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RP_{{R.name}}   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RP_{{R.name}}    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RP_{{R.name}}   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RP_{{R.name}}  YYIO_frexp2_RP_{{R.name}}_IMPL
#      define YYIO_frexp10_RP_{{R.name}} YYIO_frexp10_RP_{{R.name}}_IMPL
#      define YYIO_nextafter_RP_{{R.name}} YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RP_{{R.name}} YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RP_{{R.name}} strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RP_{{R.name}} YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YYIO_FLOAT{{V.1}}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RP_{{R.name}} YYIO_FLOAT_PRI{{V.1}}
#      endif
#      define YYIO_MAX_10_EXP_RP_{{R.name}} YYIO_MAX_10_EXP{{V.1}}
#    endif
{% if loop.last %}
#  else
#    error Representation of decimal type {{V.1}} unknown
#  endif
{% endif %}
{% endfor %}
{% else %}
/* Binary Representations */
{% for R in j_FLOATREPRS if R.name.startswith('B') %}
{% if loop.first %}#  if{% else %}#  elif{% endif %} YYIO_FLOAT_MANT_DIG{{V.1}} == {{R.mant}}
#    define YYIO_RP_OF_{{V.1}}_IS_{{R.name}} 1
#    ifndef YYIO_FLOAT_RP_{{R.name}}
#      define YYIO_FLOAT_RP_{{R.name}} YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RP_{{R.name}} YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RP_{{R.name}}   YYIO_floor{{V.1}}
#      define YYIO_modf_RP_{{R.name}}    YYIO_modf{{V.1}}
#      define YYIO_log2_RP_{{R.name}}    YYIO_log2{{V.1}}
#      define YYIO_log10_RP_{{R.name}}   YYIO_log10{{V.1}}
#      define YYIO_exp2_RP_{{R.name}}    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RP_{{R.name}}   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RP_{{R.name}}    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RP_{{R.name}}   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RP_{{R.name}}  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RP_{{R.name}} YYIO_frexp10_RP_{{R.name}}_IMPL
#      define YYIO_nextafter_RP_{{R.name}} YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RP_{{R.name}} YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RP_{{R.name}} strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RP_{{R.name}} YYIO_HAS_strfrom{{V.1}}
{% if R.name in ["B80", "B128"] %}
#      define YYIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YYIO_FLOATl
{% else %}
#      define YYIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YYIO_FLOATd
{% endif %}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RP_{{R.name}} YYIO_FLOAT_PRI{{V.1}}
#      endif
#      define YYIO_MAX_10_EXP_RP_{{R.name}} YYIO_MAX_10_EXP{{V.1}}
#    endif
{% if loop.last %}
#  else
#    error Representation of binary type {{V.1}} unknown
#  endif
{% endif %}
{% endfor %}
{% endif %}
#endif
{% endfor %}

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}
#if YIO_HAS_FLOAT{{V.1}}

{% for R in V.reprs %}
#if YYIO_RP_OF_{{V.1}}_IS_{{R}}

#  ifndef YYIO_frexp10{{V.1}}
#    define YYIO_frexp10{{V.1}}  YYIO_frexp10_RP_{{R}}
#  endif
#  ifndef YYIO_frexp2{{V.1}}
#    define YYIO_frexp2{{V.1}}   YYIO_frexp2_RP_{{R}}
#  endif

#  ifndef YYIO_float_astrfrom_naive{{V.1}}
#    define YYIO_float_astrfrom_naive{{V.1}}  YYIO_float_astrfrom_naive_{{R}}
#  endif
#  ifndef YYIO_float_astrfrom_strfrom{{V.1}}
#    define YYIO_float_astrfrom_strfrom{{V.1}} YYIO_float_astrfrom_strfrom_{{R}}
#  endif
#  ifndef YYIO_float_astrfrom_printf{{V.1}}
#    define YYIO_float_astrfrom_printf{{V.1}}  YYIO_float_astrfrom_printf_{{R}}
#  endif
#  ifndef YYIO_float_astrfrom_ryu{{V.1}}
#    define YYIO_float_astrfrom_ryu{{V.1}}     YYIO_float_astrfrom_ryu_{{R}}
#  endif
#  ifndef YYIO_has_float_astrfrom_naive{{V.1}}
#    define YYIO_has_float_astrfrom_naive{{V.1}}       YYIO_has_float_astrfrom_naive_{{R}}
#  endif
#  ifndef YYIO_has_float_astrfrom_strfrom{{V.1}}
#    define YYIO_has_float_astrfrom_strfrom{{V.1}}      YYIO_has_float_astrfrom_strfrom_{{R}}
#  endif
#  ifndef YYIO_has_float_astrfrom_printf{{V.1}}
#    define YYIO_has_float_astrfrom_printf{{V.1}}       YYIO_has_float_astrfrom_printf_{{R}}
#  endif
#  ifndef YYIO_has_float_astrfrom_ryu{{V.1}}
#    define YYIO_has_float_astrfrom_ryu{{V.1}}          YYIO_has_float_astrfrom_ryu_{{R}}
#  endif
#endif
{% endfor %}

#endif
{% endfor %}

/* ------------------------------------------------------------------------- */

#ifdef YYIO_PRIVATE
{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_RP_{{R.name}}
YYIO_FLOAT_RP_{{R.name}} YYIO_frexp10_RP_{{R.name}}_IMPL(YYIO_FLOAT_RP_{{R.name}} val, int *exp);
YYIO_FLOAT_RP_{{R.name}} YYIO_frexp2_RP_{{R.name}}_IMPL(YYIO_FLOAT_RP_{{R.name}} val, int *exp);
#endif
{% endfor %}
#endif

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_FLOAT_H_
