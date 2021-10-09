/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "private.h"

#ifdef __CDT_PARSER__
#define _yIO_HAS_DEC$1
typedef _Decimal32 _Decimal$1;
#endif

m4_applyforeachdefine(«((32, df), (64, dd), (128, dl))», «m4_dnl;

#if _yIO_HAS_DEC$1

/**
 * @def _yIO_FLOATd$1
 * A typedef to _Decimal$1 type.
 */
typedef _Decimal$1 _yIO_FLOATd$1;

/**
 * @def _yIO_FLOAT_Cd$1(x)
 * @param x A floating point value without suffix.
 * Declares a constant of _Decimal$1 type.
 */
#define _yIO_FLOAT_Cd$1(x)  x ## $2

/**
 * @def _yIO_FLOAT_PRId$1
 * The printf modifier used to print _Decimal type.
 */
#define _yIO_FLOAT_PRId$1   $3

/**
 * Like frexp(), but always with base 2.
 * @param val
 * @param exp
 * @return
 */
_Decimal$1 _yIO_frexp2d$1(_Decimal$1 val, int *exp);

/**
 * Like frexp(), but always with base 10.
 * @param val
 * @param exp
 * @return
 */
_Decimal$1 _yIO_frexp10d$1(_Decimal$1 val, int *exp);

#endif // _yIO_HAS_DEC$1

») m4_dnl m4_applyforeachdefine;
