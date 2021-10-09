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

#ifdef __CDT_PARSER__
#define _yIO_HAS_FLOAT$1  1
#define _yIO_FLOAT$1      float
#define _yIO_FLOAT_PRI$1  ""
#define $3_MANT_DIG  20
#define _yIO_FLOAT_HUGE_VALf$1  HUGE_VALf
#endif

#define _yIO_FLOATf  float
#define _yIO_FLOATd  double
#define _yIO_FLOATl  long double

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

m4_dnl suffix math_suffix MACRO_prefix
m4_applyforeachdefine(«(
		(f, f, FLT),
		(d,  , DBL),
		(l, l, LDBL)
)», m4_syncline(1)«m4_dnl;
#if _yIO_HAS_FLOAT$1

#define _yIO_frexp2$1  frexp$2
#define _yIO_floor$1   floor$2
#define _yIO_exp2$1    exp2$2
#define _yIO_log10$1   log10$2
#define _yIO_fabs$1    fabs$2
#define _yIO_pow$1     pow$2
#define _yIO_nextafter$1     nextafter$2
#define _yIO_nexttoward$1    nexttoward$2


#if _yIO_HAS_exp10$1
#define _yIO_exp10$1  exp10$2
#else
#define _yIO_exp10$1(x)  pow$2(_yIO_FLOAT_C$1(10.0), x)
#endif

_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp);

#define _yIO_strfrom$1 strfrom$1

#define _yIO_FLOAT_MANT_DIG$1  $3_MANT_DIG
#define _yIO_FLOAT_MAX$1       $3_MAX
#define _yIO_FLOAT_EPSILON$1   $3_EPSILON
#define _yIO_FLOAT_MIN$1       $3_MIN

#endif // _yIO_HAS_FLOAT$1
») m4_dnl m4_applyforeachdefine;

