/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../yio_config.h"

// {#
#ifdef __CDT_PARSER__
#define _yIO_HAS_FLOAT$1  1
#define _yIO_FLOAT$1      float
#define _yIO_FLOAT_PRI$1  ""
#define $3_MANT_DIG  20
#define _yIO_FLOAT_HUGE_VALf$1  HUGE_VALf
#endif
// #}

/* ------------------------------------------------------------------------- */

#define _yIO_FLOAT_PRIf  ""
#define _yIO_FLOAT_PRId  "l"
#define _yIO_FLOAT_PRIl  "L"

#define _yIO_FLOAT_Cf(x)  x ## f
#define _yIO_FLOAT_Cd(x)  x
#define _yIO_FLOAT_Cl(x)  x ## l

#define _yIO_FLOAT_HUGE_VALf  HUGE_VALF
#define _yIO_FLOAT_HUGE_VALd  HUGE_VAL
#define _yIO_FLOAT_HUGE_VALl  HUGE_VALL

#define _yIO_strtof  strtof
#define _yIO_strtod  strtod
#define _yIO_strtol  strtold

/* ------------------------------------------------------------------------- */

{% call j_FOREACHAPPLY([
		["d32", "DEC32", "32", "df", "\"H\"", 7],
		["d64", "DEC64", "64", "dd", "\"D\"", 16],
		["d128", "DEC128", "128", "dl", "\"DD\"", 34],
	]) %}
#line
#if _yIO_HAS_FLOAT$1
#ifdef $2_MANT_DIG
#define _yIO_FLOAT_MANT_DIG$1  $2_MANT_DIG
#else
#define _yIO_FLOAT_MANT_DIG$1  $6
#endif
#define _yIO_FLOAT_C$1(x)  (x ## $4)
#define _yIO_FLOAT_PRI$1   $5
#endif
{% endcall %}

/* ------------------------------------------------------------------------- */

{% macro j_floatdefine() %}{% call j_APPLY(*varargs) %}
#line
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

_yIO_FLOAT$1 frexp$2(_yIO_FLOAT$1, int *);

/**
 * @define _yIO_frexp2$1
 * Like frexp(), but always with base 2.
 */
{% if varargs.3 is defined %}
#line
_yIO_FLOAT$1 _yIO_frexp2$1(_yIO_FLOAT$1, int *);
{% else %}
#line
#define _yIO_frexp2$1  frexp$2
{% endif %}
#line

/**
 * @define _yIO_frexp10$1
 * Like frexp(), but always with base 10.
 */
{% if varargs.3 is defined %}
#line
#define _yIO_frexp10$1  frexp$2
{% else %}
#line
_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1, int *);
{% endif %}
#line

_yIO_FLOAT$1 floor$2(_yIO_FLOAT$1);
#define _yIO_floor$1   floor$2
_yIO_FLOAT$1 exp2$2(_yIO_FLOAT$1);
#define _yIO_exp2$1    exp2$2
_yIO_FLOAT$1 log10$2(_yIO_FLOAT$1);
#define _yIO_log10$1   log10$2
_yIO_FLOAT$1 fabs$2(_yIO_FLOAT$1);
#define _yIO_fabs$1    fabs$2
_yIO_FLOAT$1 pow$2(_yIO_FLOAT$1, _yIO_FLOAT$1);
#define _yIO_pow$1     pow$2
_yIO_FLOAT$1 nextafter$2(_yIO_FLOAT$1, _yIO_FLOAT$1);
#define _yIO_nextafter$1  nextafter$2

#ifndef _yIO_FLOAT_C$1
#define _yIO_FLOAT_C$1(x)  ((_yIO_FLOAT$1)x)
#endif

#if _yIO_HAS_exp10$1
_yIO_FLOAT$1 exp10$2(_yIO_FLOAT$1);
#define _yIO_exp10$1  exp10$2
#else
static inline
_yIO_FLOAT$1 _yIO_exp10$1(_yIO_FLOAT$1 x) {
	return pow$2(_yIO_FLOAT_C$1(10.0), x);
}
#endif

#ifndef _yIO_strto$1
#define _yIO_strto$1  strto$1
#endif

#ifndef _yIO_FLOAT_MANT_DIG$1
#ifdef $3_MANT_DIG
#define _yIO_FLOAT_MANT_DIG$1  $3_MANT_DIG
#elif defined(__$3_MANT_DIG__)
#define _yIO_FLOAT_MANT_DIG$1  __$3_MANT_DIG__
#else
#error Can not define _yIO_FLOAT_MANT_DIG$1
#endif
#endif

#define _yIO_FLOAT_MAX$1       $3_MAX
#define _yIO_FLOAT_EPSILON$1   $3_EPSILON
#define _yIO_FLOAT_MIN$1       $3_MIN


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

