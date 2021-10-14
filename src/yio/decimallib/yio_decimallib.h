/**
 * @file
 * @date 2020-06-11
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "yio/yio_config.h"
#include <stddef.h>

#if __CDT_PARSER__
#define _yIO_HAS_DEC$1
#define _Decimal$1 _Decimal32
#endif

m4_applyforeachdefine(«(
			(d32,  32),
			(d64,  64),
			(d128, 128),
)», m4_syncline()«m4_dnl;

#if _yIO_HAS_FLOAT$1

_Decimal$2 floor$1(_Decimal$2 x);
_Decimal$2 fabs$1(_Decimal$2 x);
_Decimal$2 exp2$1(_Decimal$2 y);
_Decimal$2 exp10$1(_Decimal$2 y);
_Decimal$2 round$1(_Decimal$2 y);
_Decimal$2 log2$1(_Decimal$2 x);
_Decimal$2 log10$1(_Decimal$2 y);
_Decimal$2 frexp$1(_Decimal$2 val, int *exp);
int strfrom$1(char *restrict str, size_t n, const char *restrict format, _Decimal$2 fp);

#endif

») // m4_dnl m4_applyforeachdefine ;
