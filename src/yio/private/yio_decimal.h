/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
// #define __STDC_WANT_IEC_60559_TYPES_EXT__
#include "../decimallib/yio_decimallib.h"

#ifdef __CDT_PARSER__
#define _yIO_HAS_FLOAT$1
typedef _Decimal32 _Decimal$1;
#endif


m4_applyforeachdefine(«(
	(d32,  32, df,  "H", 7),
	(d64,  64, dd,  "D", 16),
	(d128, 128, dl, "DD", 34),
)», m4_syncline()«

#if _yIO_HAS_FLOAT$1

/**
 * @def _yIO_FLOAT$1
 * A typedef to _Decimal$1 type.
 */
typedef _Decimal$2 _yIO_FLOAT$1;

/**
 * @def _yIO_FLOAT_C$1(x)
 * @param x A floating point value without suffix.
 * Declares a constant of _Decimal$1 type.
 */
#define _yIO_FLOAT_C$1(x)  x ## $3

/**
 * @def _yIO_FLOAT_PRI$1
 * The printf modifier used to print _Decimal type.
 */
#define _yIO_FLOAT_PRI$1   $4

/**
 * Like frexp(), but always with base 2.
 * @param val
 * @param exp
 * @return
 */
_yIO_FLOAT$1 _yIO_frexp2$1(_yIO_FLOAT$1 val, int *exp);

/**
 * Like frexp(), but always with base 10.
 * @param val
 * @param exp
 * @return
 */
_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp);

#define _yIO_FLOAT_MANT_DIG$1 $5
#define _yIO_fabs$1     fabs$1
#define _yIO_exp2$1     exp2$1
#define _yIO_exp10$1    exp10$1


#endif

»)
