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
#define _yIO_HAS_FLOAT$1
#define _yIO_FLOAT$1 float
#define _yIO_FLOAT_PRI$1 ""
#define _yIO_FLOAT_C$1(x) x ## f
#define _yIO_strfrom$1 strfromf
#define _yIO_FLOAT$1 float
#endif

#define _yIO_FLOATf  float
#define _yIO_FLOAT   double
#define _yIO_FLOATl  long double

#define _yIO_FLOAT_PRIf  ""
#define _yIO_FLOAT_PRI   "l"
#define _yIO_FLOAT_PRIl  "L"

#define _yIO_FLOAT_Cf(x)  x ## f
#define _yIO_FLOAT_C(x)   ((double)(x))
#define _yIO_FLOAT_Cl(x)  x ## l

#define _yIO_strfromf  strfromf
#define _yIO_strfrom   strfromd // YES! YES! YES!
#define _yIO_strfroml  strfroml

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;
#ifdef _yIO_HAS_FLOAT$1
_yIO_FLOAT$1 _yIO_frexp2$1(_yIO_FLOAT$1 val, int *exp);
_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp);
#endif // _yIO_HAS_FLOAT$1
~) m4_dnl m4_applyforeachdefine;

