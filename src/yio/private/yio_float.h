/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_FLOAT_H_
#define YYIO_YIO_PRIVATE_YIO_FLOAT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include <float.h>

// {#
#ifdef YYIO_CDT_PARSER
#define YIO_HAS_FLOAT$1  1
#define YYIO_FLOAT$1      float
#define YYIO_FLOAT_PRI$1  ""
#define $3_MANT_DIG  20
#define YYIO_FLOAT_HUGE_VALf$1  HUGE_VALf
#endif
// #}

/* ------------------------------------------------------------------------- */

#define YYIO_FLOAT_PRIf  ""
#define YYIO_FLOAT_PRId  "l"
#define YYIO_FLOAT_PRIl  "L"

#define YYIO_FLOAT_Cf(x)  x ## f
#define YYIO_FLOAT_Cd(x)  x
#define YYIO_FLOAT_Cl(x)  x ## l

#define YYIO_FLOAT_HUGE_VALf  HUGE_VALF
#define YYIO_FLOAT_HUGE_VALd  HUGE_VAL
#define YYIO_FLOAT_HUGE_VALl  HUGE_VALL

#define YYIO_strtof  strtof
#define YYIO_strtod  strtod
#define YYIO_strtol  strtold

/* ------------------------------------------------------------------------- */

{% call j_FOREACHAPPLY([
		["d32", "DEC32", "32", "df", "\"H\"", 7],
		["d64", "DEC64", "64", "dd", "\"D\"", 16],
		["d128", "DEC128", "128", "dl", "\"DD\"", 34],
	]) %}
#line
#if YIO_HAS_FLOAT$1
#ifdef __$2_MANT_DIG__
#define YYIO_FLOAT_MANT_DIG$1  __$2_MANT_DIG__
#elif defined($2_MANT_DIG)
#define YYIO_FLOAT_MANT_DIG$1  $2_MANT_DIG
#else
#define YYIO_FLOAT_MANT_DIG$1  $6
#endif
#define YYIO_FLOAT_C$1(x)  (x ## $4)
#define YYIO_FLOAT_PRI$1   $5
#endif
{% endcall %}

/* ------------------------------------------------------------------------- */

{% macro j_floatdefine() %}{% call j_APPLY(*varargs) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

#define YYIO_IF_FLOAT$1(...)  __VA_ARGS__

#ifdef __cplusplus
{% if "$G" != "s" %}
#define YYIO_SKIP_FLOAT_DECL$1
{% endif %}
#endif

#ifndef YYIO_SKIP_FLOAT_DECL$1

YYIO_FLOAT$1 frexp$2(YYIO_FLOAT$1 x, int *exp);

/**
 * @define YYIO_frexp2$1
 * Like frexp(), but always with base 2.
 */
{% if varargs.3 is defined %}
#line
YYIO_FLOAT$1 YYIO_frexp2$1(YYIO_FLOAT$1 x, int *exp);
{% else %}
#line
#define YYIO_frexp2$1  frexp$2
{% endif %}
#line

/**
 * @define YYIO_frexp10$1
 * Like frexp(), but always with base 10.
 */
#line
YYIO_FLOAT$1 YYIO_frexp10$1(YYIO_FLOAT$1 x, int *exp);
#line

YYIO_FLOAT$1 floor$2(YYIO_FLOAT$1 x);
#define YYIO_floor$1   floor$2
YYIO_FLOAT$1 exp2$2(YYIO_FLOAT$1 x);
#define YYIO_exp2$1    exp2$2
YYIO_FLOAT$1 log2$2(YYIO_FLOAT$1 x);
#define YYIO_log2$1    log2$2
YYIO_FLOAT$1 log10$2(YYIO_FLOAT$1 x);
#define YYIO_log10$1   log10$2
YYIO_FLOAT$1 fabs$2(YYIO_FLOAT$1 x);
#define YYIO_fabs$1    fabs$2
YYIO_FLOAT$1 pow$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#define YYIO_pow$1     pow$2
YYIO_FLOAT$1 nextafter$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#define YYIO_nextafter$1  nextafter$2
YYIO_FLOAT$1 modf$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 *iptr);
#define YYIO_modf$1       modf$2

#ifndef YYIO_FLOAT_C$1
#define YYIO_FLOAT_C$1(x)  ((YYIO_FLOAT$1)x)
#endif

#if YYIO_HAS_exp10$1
YYIO_FLOAT$1 exp10$2(YYIO_FLOAT$1 x);
#define YYIO_exp10$1  exp10$2
#else
static inline
YYIO_FLOAT$1 YYIO_exp10$1(YYIO_FLOAT$1 x) {
	return pow$2(YYIO_FLOAT_C$1(10.0), x);
}
#endif

#ifndef YYIO_strto$1
#define YYIO_strto$1  strto$1
#endif

#ifndef YYIO_FLOAT_MANT_DIG$1
#ifdef $3_MANT_DIG
#define YYIO_FLOAT_MANT_DIG$1  $3_MANT_DIG
#elif defined(__$3_MANT_DIG__)
#define YYIO_FLOAT_MANT_DIG$1  __$3_MANT_DIG__
#else
#error Can not define YYIO_FLOAT_MANT_DIG$1
#endif
#endif

#define YYIO_FLOAT_MAX$1       $3_MAX
#define YYIO_FLOAT_EPSILON$1   $3_EPSILON
#define YYIO_FLOAT_MIN$1       $3_MIN

#endif

#else

#define YYIO_IF_FLOAT$1(...)

#endif
{% endcall %}{% endmacro %}

/* ------------------------------------------------------------------------- */

{% call(V) j_FOREACHAPPLY([
		["f", "", "FLT"],
		["d", "", "DBL"],
		["l", "", "LDBL"] ]) %}

#if YIO_HAS_FLOAT$1
#ifndef YYIO_MAX_10_EXP$1
#ifdef $3_MAX_10_EXP
#define YYIO_MAX_10_EXP$1  $3_MAX_10_EXP
#elif defined(__$3_MAX_10_EXP__)
#define YYIO_MAX_10_EXP$1  __$3_MAX_10_EXP__
#else
#error Can not define YYIO_MAX_10_EXP$1 for type $2
#endif
#endif
#endif

{% endcall %}

{% for v in [
		["f", "FLT"],
		["d", "DBL"],
		["l", "LDBL"],
		["f16", "FLT16"],
		["f32", "FLT32"],
		["f64", "FLT64"],
		["f128", "FLT128"],
		["f32x", "FLT32X"],
		["f64x", "FLT64X"],
		["f128x", "FLT128X"],
	] %}
#line
{% set j_mathsuffix = "" if v.0 == "d" else v.0 %}

{{ j_floatdefine(v.0, j_mathsuffix, v.1) }}

{% endfor %}

/* ------------------------------------------------------------------------- */

{% for v in [
	"d32", "d64", "d128",
	"d32x", "d64x", "d128x",
	] %}
#line
{% set j_CONSTPREFIX = "DEC" + v|replace('d','')|upper %}
{{ j_floatdefine(v, v, j_CONSTPREFIX, "DEC") }}
{% endfor %}

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}
#line
#if YIO_HAS_FLOAT{{V.1}}
#ifdef __cplusplus
{% if V.1 not in ["f", "d", "l"] %}
#undef YIO_HAS_FLOAT{{V.1}}
#define YIO_HAS_FLOAT{{V.1}} 0
{% endif %}
#endif
#endif

#if YIO_HAS_FLOAT{{V.1}}
/* Binary Representations */
#  if YYIO_FLOAT_MANT_DIG{{V.1}} == 11
#    define YYIO_REPR_OF_{{V.1}}_IS_B16 1
#    ifndef YYIO_FLOAT_REPR_B16
#      define YYIO_FLOAT_REPR_B16 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_B16 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_B16   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_B16    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_B16    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_B16   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_B16    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_B16   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_B16    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_B16   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_B16  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RC_B16 YYIO_frexp10_RC_B16_IMPL
#      define YYIO_nextafter_RC_B16 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_B16 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_B16 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_B16 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B16 double
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_B16 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_B16 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_B16 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 24
#    define YYIO_REPR_OF_{{V.1}}_IS_B32 1
#    ifndef YYIO_FLOAT_REPR_B32
#      define YYIO_FLOAT_REPR_B32 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_B32 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_B32   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_B32    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_B32    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_B32   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_B32    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_B32   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_B32    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_B32   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_B32  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RC_B32 YYIO_frexp10_RC_B32_IMPL
#      define YYIO_nextafter_RC_B32 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_B32 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_B32 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_B32 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B32 double
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_B32 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_B32 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_B32 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 53
#    define YYIO_REPR_OF_{{V.1}}_IS_B64 1
#    ifndef YYIO_FLOAT_REPR_B64
#      define YYIO_FLOAT_REPR_B64 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_B64 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_B64   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_B64    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_B64    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_B64   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_B64    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_B64   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_B64    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_B64   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_B64  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RC_B64 YYIO_frexp10_RC_B64_IMPL
#      define YYIO_nextafter_RC_B64 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_B64 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_B64 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_B64 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B64 double
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_B64 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_B64 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_B64 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 64
#    define YYIO_REPR_OF_{{V.1}}_IS_B80 1
#    ifndef YYIO_FLOAT_REPR_B80
#      define YYIO_FLOAT_REPR_B80 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_B80 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_B80   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_B80    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_B80    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_B80   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_B80    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_B80   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_B80    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_B80   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_B80  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RC_B80 YYIO_frexp10_RC_B80_IMPL
#      define YYIO_nextafter_RC_B80 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_B80 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_B80 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_B80 YYIO_HAS_strfrom{{V.1}}
{% if V.math == 'l' %}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B80 long double
{% else %}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B80 double
{% endif %}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_B80 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_B80 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_B80 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 113
#    define YYIO_REPR_OF_{{V.1}}_IS_B128 1
#    ifndef YYIO_FLOAT_REPR_B128
#      define YYIO_FLOAT_REPR_B128 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_B128 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_B128   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_B128    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_B128    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_B128   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_B128    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_B128   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_B128    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_B128   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_B128  YYIO_frexp2{{V.1}}
#      define YYIO_frexp10_RC_B128 YYIO_frexp10_RC_B128_IMPL
#      define YYIO_nextafter_RC_B128 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_B128 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_B128 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_B128 YYIO_HAS_strfrom{{V.1}}
{% if V.math == 'l' %}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B128 long double
{% else %}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_B128 double
{% endif %}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_B128 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_B128 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_B128 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  endif
/* Decimal Representations */
#  if YYIO_FLOAT_MANT_DIG{{V.1}} == 7
#    define YYIO_REPR_OF_{{V.1}}_IS_D32 1
#    ifndef YYIO_FLOAT_REPR_D32
#      define YYIO_FLOAT_REPR_D32 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_D32 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_D32   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_D32    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_D32    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_D32   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_D32    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_D32   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_D32    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_D32   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_D32  YYIO_frexp2_RC_D32_IMPL
#      define YYIO_frexp10_RC_D32 YYIO_frexp10_RC_D32_IMPL
#      define YYIO_nextafter_RC_D32 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_D32 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_D32 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_D32 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_D32 YYIO_FLOAT{{V.1}}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_D32 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_D32 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_D32 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 16
#    define YYIO_REPR_OF_{{V.1}}_IS_D64 1
#    ifndef YYIO_FLOAT_REPR_D64
#      define YYIO_FLOAT_REPR_D64 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_D64 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_D64   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_D64    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_D64    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_D64   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_D64    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_D64   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_D64    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_D64   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_D64  YYIO_frexp2_RC_D64_IMPL
#      define YYIO_frexp10_RC_D64 YYIO_frexp10_RC_D64_IMPL
#      define YYIO_nextafter_RC_D64 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_D64 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_D64 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_D64 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_D64 YYIO_FLOAT{{V.1}}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_D64 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_D64 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_D64 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  elif YYIO_FLOAT_MANT_DIG{{V.1}} == 34
#    define YYIO_REPR_OF_{{V.1}}_IS_D128 1
#    ifndef YYIO_FLOAT_REPR_D128
#      define YYIO_FLOAT_REPR_D128 YYIO_FLOAT{{V.1}}
#      define YYIO_FLOAT_MANT_DIG_RC_D128 YYIO_FLOAT_MANT_DIG{{V.1}}
#      define YYIO_floor_RC_D128   YYIO_floor{{V.1}}
#      define YYIO_modf_RC_D128    YYIO_modf{{V.1}}
#      define YYIO_log2_RC_D128    YYIO_log2{{V.1}}
#      define YYIO_log10_RC_D128   YYIO_log10{{V.1}}
#      define YYIO_exp2_RC_D128    YYIO_exp2{{V.1}}
#      define YYIO_exp10_RC_D128   YYIO_exp10{{V.1}}
#      define YYIO_fabs_RC_D128    YYIO_fabs{{V.1}}
#      define YYIO_frexp_RC_D128   YYIO_frexp{{V.1}}
#      define YYIO_frexp2_RC_D128  YYIO_frexp2_RC_D128_IMPL
#      define YYIO_frexp10_RC_D128 YYIO_frexp10_RC_D128_IMPL
#      define YYIO_nextafter_RC_D128 YYIO_nextafter{{V.1}}
#      define YYIO_FLOAT_C_RC_D128 YYIO_FLOAT_C{{V.1}}
#      define YYIO_strfrom_RC_D128 strfrom{{V.1}}
#      define YYIO_HAS_strfrom_RC_D128 YYIO_HAS_strfrom{{V.1}}
#      define YYIO_FLOAT_PRINTF_TYPE_RC_D128 YYIO_FLOAT{{V.1}}
#      ifdef YYIO_FLOAT_PRI{{V.1}}
#        define YYIO_FLOAT_PRI_RC_D128 YYIO_FLOAT_PRI{{V.1}}
#      else
#        define YYIO_FLOAT_PRI_RC_D128 ""
#      endif
#      define YYIO_MAX_10_EXP_RC_D128 YYIO_MAX_10_EXP{{V.1}}
#    endif
#  endif
#endif
{% endfor %}

/* ------------------------------------------------------------------------- */

{% for V in j_FLOATS %}
#if YIO_HAS_FLOAT{{V.1}}

{% for R in V.reprs %}
#if YYIO_REPR_OF_{{V.1}}_IS_{{R}}

#  ifndef YYIO_frexp10{{V.1}}
#    define YYIO_frexp10{{V.1}}  YYIO_frexp10_RC_{{R}}
#  endif
#  ifndef YYIO_frexp2{{V.1}}
#    define YYIO_frexp2{{V.1}}   YYIO_frexp2_RC_{{R}}
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
#  ifndef YYIO_has_float_naive{{V.1}}
#    define YYIO_has_float_naive{{V.1}}       YYIO_has_float_naive_{{R}}
#  endif
#  ifndef YYIO_has_float_strfrom{{V.1}}
#    define YYIO_has_float_strfrom{{V.1}}      YYIO_has_float_strfrom_{{R}}
#  endif
#  ifndef YYIO_has_float_printf{{V.1}}
#    define YYIO_has_float_printf{{V.1}}       YYIO_has_float_printf_{{R}}
#  endif
#  ifndef YYIO_has_float_ryu{{V.1}}
#    define YYIO_has_float_ryu{{V.1}}          YYIO_has_float_ryu_{{R}}
#  endif
#endif
{% endfor %}

#endif
{% endfor %}

/* ------------------------------------------------------------------------- */

#ifdef YYIO_PRIVATE
{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_REPR_{{R}}
YYIO_FLOAT_REPR_{{R}} YYIO_frexp10_RC_{{R}}_IMPL(YYIO_FLOAT_REPR_{{R}} val, int *exp);
YYIO_FLOAT_REPR_{{R}} YYIO_frexp2_RC_{{R}}_IMPL(YYIO_FLOAT_REPR_{{R}} val, int *exp);
#endif
{% endfor %}
#endif

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_FLOAT_H_
