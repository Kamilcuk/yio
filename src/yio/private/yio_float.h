// NOLINE
/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief Floating point type definitions.
 */
#ifndef YIO_YIO_PRIVATE_YIO_FLOAT_H_
#define YIO_YIO_PRIVATE_YIO_FLOAT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include <stddef.h>
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
#define YIO_FLOAT_C$1(x)  (x{{ " ## " + v.CS_VAL if v.CS_VAL else "" }})
#define YIO_FLOAT_PRI$1   {{v.PRI_VAL}}
{{ j_robust_define("YIO_FLOAT_MANT_DIG$1", v.CONSTPREF + "_MANT_DIG", v.MANT_VAL) }}
#ifdef INFINITY
{{ j_robust_define("YIO_FLOAT_HUGE_VAL$1", "HUGE_VAL" + v.HUGE_VAL_SUFF, "((YIO_FLOAT$1)INFINITY)") }}
#else
{{ j_robust_define("YIO_FLOAT_HUGE_VAL$1", "HUGE_VAL" + v.HUGE_VAL_SUFF) }}
#endif
{{ j_robust_define("YIO_FLOAT_MAX$1", "$3_MAX") }}
{{ j_robust_define("YIO_FLOAT_EPSILON$1", "$3_EPSILON") }}
{{ j_robust_define("YIO_FLOAT_MIN$1", "$3_MIN") }}
{{ j_robust_define("YIO_MAX_10_EXP$1", "$3_MAX_10_EXP", v.EXP10_VAL) }}


/**
 * Like frexp(), but always with base 2.
 */
{% if v.G.startswith('d') %}
YIO_FLOAT$1 YIO_frexp2$1(YIO_FLOAT$1 x, int *exp);
{% else %}
#define YIO_frexp2$1  frexp$2
{% endif %}

#ifndef frexp$2
YIO_FLOAT$1 frexp$2(YIO_FLOAT$1 x, int *exp);
#endif
#define YIO_frexp$1     frexp$2

/// Like frexp(), but always with base 10.
YIO_FLOAT$1 YIO_frexp10$1(YIO_FLOAT$1 x, int *exp);

#ifndef floor$2
YIO_FLOAT$1 floor$2(YIO_FLOAT$1 x);
#endif
#define YIO_floor$1   floor$2

#ifndef exp2$2
YIO_FLOAT$1 exp2$2(YIO_FLOAT$1 x);
#endif
#define YIO_exp2$1    exp2$2

#ifndef log2$2
YIO_FLOAT$1 log2$2(YIO_FLOAT$1 x);
#endif
#define YIO_log2$1    log2$2

#ifndef log10$2
YIO_FLOAT$1 log10$2(YIO_FLOAT$1 x);
#endif
#define YIO_log10$1   log10$2

#ifndef fabs$2
YIO_FLOAT$1 fabs$2(YIO_FLOAT$1 x);
#endif
#define YIO_fabs$1    fabs$2

#ifndef pow$2
YIO_FLOAT$1 pow$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y);
#endif
#define YIO_pow$1     pow$2

#ifndef nextafter$2
YIO_FLOAT$1 nextafter$2(YIO_FLOAT$1 x, YIO_FLOAT$1 y);
#endif
#define YIO_nextafter$1  nextafter$2

#ifndef modf$2
YIO_FLOAT$1 modf$2(YIO_FLOAT$1 x, YIO_FLOAT$1 *iptr);
#endif
#define YIO_modf$1       modf$2

#ifndef YIO_HAS_exp10$1
#error YIO_HAS_exp10$1 is not defined
#endif
#if YIO_HAS_exp10$1
#ifndef exp10$2
YIO_FLOAT$1 exp10$2(YIO_FLOAT$1 x);
#endif
#define YIO_exp10$1  exp10$2
#else
static inline
YIO_FLOAT$1 YIO_exp10$1(YIO_FLOAT$1 x) {
	return pow$2(YIO_FLOAT_C$1(10.0), x);
}
#endif

#define YIO_strto$1  strto{{v.strto}}

#if YIO_HAS_strfrom$1
extern int strfrom{{v.math}}(char *restrict str, size_t n, const char *restrict format, YIO_FLOAT$1 fp);
#endif

#if YIO_HAS_strto$1
extern YIO_FLOAT$1 strto{{v.strto}}(const char *restrict str, char **restrict pnt);
#endif

#endif
{% endcall %}
{% endcall %}

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}
#if YIO_HAS_FLOAT{{V.1}}
{% if j_search(V.1, "^d[0-9]") %}
/* Decimal Representations */
{% for R in j_FLOATREPRS if R.name.startswith('D') %}
{% if loop.first %}#  if{% else %}#  elif{% endif %} YIO_FLOAT_MANT_DIG{{V.1}} == {{R.mant}}
#    define YIO_RP_OF_{{V.1}}_IS_{{R.name}} 1
#    ifndef YIO_FLOAT_RP_{{R.name}}
#      define YIO_FLOAT_RP_{{R.name}} YIO_FLOAT{{V.1}}
#      define YIO_FLOAT_MANT_DIG_RP_{{R.name}} YIO_FLOAT_MANT_DIG{{V.1}}
#      define YIO_floor_RP_{{R.name}}   YIO_floor{{V.1}}
#      define YIO_modf_RP_{{R.name}}    YIO_modf{{V.1}}
#      define YIO_log2_RP_{{R.name}}    YIO_log2{{V.1}}
#      define YIO_log10_RP_{{R.name}}   YIO_log10{{V.1}}
#      define YIO_exp2_RP_{{R.name}}    YIO_exp2{{V.1}}
#      define YIO_exp10_RP_{{R.name}}   YIO_exp10{{V.1}}
#      define YIO_fabs_RP_{{R.name}}    YIO_fabs{{V.1}}
#      define YIO_frexp_RP_{{R.name}}   YIO_frexp{{V.1}}
#      define YIO_frexp2_RP_{{R.name}}  YIO_frexp2_RP_{{R.name}}_IMPL
#      define YIO_frexp10_RP_{{R.name}} YIO_frexp10_RP_{{R.name}}_IMPL
#      define YIO_nextafter_RP_{{R.name}} YIO_nextafter{{V.1}}
#      define YIO_FLOAT_C_RP_{{R.name}} YIO_FLOAT_C{{V.1}}
#      define YIO_strfrom_RP_{{R.name}} strfrom{{V.1}}
#      define YIO_HAS_strfrom_RP_{{R.name}} YIO_HAS_strfrom{{V.1}}
#      define YIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YIO_FLOAT{{V.1}}
#      ifdef YIO_FLOAT_PRI{{V.1}}
#        define YIO_FLOAT_PRI_RP_{{R.name}} YIO_FLOAT_PRI{{V.1}}
#      endif
#      define YIO_MAX_10_EXP_RP_{{R.name}} YIO_MAX_10_EXP{{V.1}}
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
{% if loop.first %}#  if{% else %}#  elif{% endif %} YIO_FLOAT_MANT_DIG{{V.1}} == {{R.mant}}
#    define YIO_RP_OF_{{V.1}}_IS_{{R.name}} 1
#    ifndef YIO_FLOAT_RP_{{R.name}}
#      define YIO_FLOAT_RP_{{R.name}} YIO_FLOAT{{V.1}}
#      define YIO_FLOAT_MANT_DIG_RP_{{R.name}} YIO_FLOAT_MANT_DIG{{V.1}}
#      define YIO_floor_RP_{{R.name}}   YIO_floor{{V.1}}
#      define YIO_modf_RP_{{R.name}}    YIO_modf{{V.1}}
#      define YIO_log2_RP_{{R.name}}    YIO_log2{{V.1}}
#      define YIO_log10_RP_{{R.name}}   YIO_log10{{V.1}}
#      define YIO_exp2_RP_{{R.name}}    YIO_exp2{{V.1}}
#      define YIO_exp10_RP_{{R.name}}   YIO_exp10{{V.1}}
#      define YIO_fabs_RP_{{R.name}}    YIO_fabs{{V.1}}
#      define YIO_frexp_RP_{{R.name}}   YIO_frexp{{V.1}}
#      define YIO_frexp2_RP_{{R.name}}  YIO_frexp2{{V.1}}
#      define YIO_frexp10_RP_{{R.name}} YIO_frexp10_RP_{{R.name}}_IMPL
#      define YIO_nextafter_RP_{{R.name}} YIO_nextafter{{V.1}}
#      define YIO_FLOAT_C_RP_{{R.name}} YIO_FLOAT_C{{V.1}}
#      define YIO_strfrom_RP_{{R.name}} strfrom{{V.1}}
#      define YIO_HAS_strfrom_RP_{{R.name}} YIO_HAS_strfrom{{V.1}}
{% if R.name in ["B80", "B128"] %}
#      define YIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YIO_FLOATl
{% else %}
#      define YIO_FLOAT_PRINTF_TYPE_RP_{{R.name}} YIO_FLOATd
{% endif %}
#      ifdef YIO_FLOAT_PRI{{V.1}}
#        define YIO_FLOAT_PRI_RP_{{R.name}} YIO_FLOAT_PRI{{V.1}}
#      endif
#      define YIO_MAX_10_EXP_RP_{{R.name}} YIO_MAX_10_EXP{{V.1}}
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
#if YIO_RP_OF_{{V.1}}_IS_{{R}}

#  ifndef YIO_frexp10{{V.1}}
#    define YIO_frexp10{{V.1}}  YIO_frexp10_RP_{{R}}
#  endif
#  ifndef YIO_frexp2{{V.1}}
#    define YIO_frexp2{{V.1}}   YIO_frexp2_RP_{{R}}
#  endif

#  ifndef YIO_float_astrfrom_naive{{V.1}}
#    define YIO_float_astrfrom_naive{{V.1}}  YIO_float_astrfrom_naive_{{R}}
#  endif
#  ifndef YIO_float_astrfrom_strfrom{{V.1}}
#    define YIO_float_astrfrom_strfrom{{V.1}} YIO_float_astrfrom_strfrom_{{R}}
#  endif
#  ifndef YIO_float_astrfrom_printf{{V.1}}
#    define YIO_float_astrfrom_printf{{V.1}}  YIO_float_astrfrom_printf_{{R}}
#  endif
#  ifndef YIO_float_astrfrom_ryu{{V.1}}
#    define YIO_float_astrfrom_ryu{{V.1}}     YIO_float_astrfrom_ryu_{{R}}
#  endif
#  ifndef YIO_has_float_astrfrom_naive{{V.1}}
#    define YIO_has_float_astrfrom_naive{{V.1}}       YIO_has_float_astrfrom_naive_{{R}}
#  endif
#  ifndef YIO_has_float_astrfrom_strfrom{{V.1}}
#    define YIO_has_float_astrfrom_strfrom{{V.1}}      YIO_has_float_astrfrom_strfrom_{{R}}
#  endif
#  ifndef YIO_has_float_astrfrom_printf{{V.1}}
#    define YIO_has_float_astrfrom_printf{{V.1}}       YIO_has_float_astrfrom_printf_{{R}}
#  endif
#  ifndef YIO_has_float_astrfrom_ryu{{V.1}}
#    define YIO_has_float_astrfrom_ryu{{V.1}}          YIO_has_float_astrfrom_ryu_{{R}}
#  endif
#endif
{% endfor %}

#endif
{% endfor %}

/* ------------------------------------------------------------------------- */

#ifdef YIO_PRIVATE
{% for R in j_FLOATREPRS %}
#line
#ifdef YIO_FLOAT_RP_{{R.name}}
YIO_FLOAT_RP_{{R.name}} YIO_frexp10_RP_{{R.name}}_IMPL(YIO_FLOAT_RP_{{R.name}} val, int *exp);
YIO_FLOAT_RP_{{R.name}} YIO_frexp2_RP_{{R.name}}_IMPL(YIO_FLOAT_RP_{{R.name}} val, int *exp);
#endif
{% endfor %}
#endif

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_FLOAT_H_
