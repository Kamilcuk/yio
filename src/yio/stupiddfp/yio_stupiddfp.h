/**
 * @file yio_stupiddfp.h
 * @date 2020-06-11
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config();
#include <yio/yio_config.h>

#if __CDT_PARSER__
#define _yIO_HAS_DEC$1
#define _Decimal$1 _Decimal32
#endif

m4_applyforeachdefine(
`(
		( 32, df,  "H"),
		( 64, dd,  "D"),
		(128, dl, "DD")
)~,`m4_dnl;

#ifdef _yIO_HAS_DEC$1

_Decimal$1 floord$1(_Decimal$1 x);
_Decimal$1 fabsd$1(_Decimal$1 x);
_Decimal$1 _yIO_expintXd$1(_Decimal$1 y, _Decimal$1 X);
_Decimal$1 exp2d$1(_Decimal$1 y);
_Decimal$1 exp10d$1(_Decimal$1 y);
_Decimal$1 roundd$1(_Decimal$1 y);
_Decimal$1 log2d$1(_Decimal$1 x);
_Decimal$1 log10d$1(_Decimal$1 y);
_Decimal$1 frexpd$1(_Decimal$1 val, int *exp);

#endif // _yIO_HAS_DEC$1

~) // m4_dnl m4_applyforeachdefine ;
