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
#include "../yio_config.h"

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
#ifdef $2_MANT_DIG
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
{% if varargs.3 is defined %}
#line
#define YYIO_frexp10$1  frexp$2
{% else %}
#line
YYIO_FLOAT$1 YYIO_frexp10$1(YYIO_FLOAT$1 x, int *exp);
{% endif %}
#line

YYIO_FLOAT$1 floor$2(YYIO_FLOAT$1 x);
#define YYIO_floor$1   floor$2
YYIO_FLOAT$1 exp2$2(YYIO_FLOAT$1 x);
#define YYIO_exp2$1    exp2$2
YYIO_FLOAT$1 log10$2(YYIO_FLOAT$1 x);
#define YYIO_log10$1   log10$2
YYIO_FLOAT$1 fabs$2(YYIO_FLOAT$1 x);
#define YYIO_fabs$1    fabs$2
YYIO_FLOAT$1 pow$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#define YYIO_pow$1     pow$2
YYIO_FLOAT$1 nextafter$2(YYIO_FLOAT$1 x, YYIO_FLOAT$1 y);
#define YYIO_nextafter$1  nextafter$2

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
{% endcall %}{% endmacro %}

/* ------------------------------------------------------------------------- */

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

#endif /* YYIO_YIO_PRIVATE_YIO_FLOAT_H_ */
