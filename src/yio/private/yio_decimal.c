/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include "yio_decimal.h"
#include <yio/yio_config.h>
#include <math.h>

m4_applyforeachdefine(`((32, df), (64, dd), (128, dl))~, `m4_dnl;

#ifdef _yIO_HAS_DEC$1

#ifndef __STDC_DEC_FP__
// Forward declaration so we get link time errors.
// If you see link errors, link with an implementation of decimal-library.
_Decimal$1 floord$1(_Decimal$1 x);
_Decimal$1 log2d$1(_Decimal$1 x);
_Decimal$1 exp2d$1(_Decimal$1 x);
_Decimal$1 frexpd$1(_Decimal$1 x, int *);
#endif // __STDC_DEC_FP__

_Decimal$1 _yIO_frexp2d$1(_Decimal$1 val, int *exp) {
	if (val == _yIO_FLOAT_Cd$1(0.0)) {
		*exp = _yIO_FLOAT_Cd$1(0.0);
	} else {
		const int tmp = 1 + floord$1(log2d$1(val));
		*exp = tmp;
		val *= exp2d$1(-tmp);
		if (val < _yIO_FLOAT_Cd$1(0.5)) {
			val = _yIO_FLOAT_Cd$1(0.5);
		} else if (val > _yIO_FLOAT_Cd$1(1.0)) {
			val = _yIO_FLOAT_Cd$1(1.0);
		}
	}
	return val;
}

_Decimal$1 _yIO_frexp10d$1(_Decimal$1 val, int *exp) {
	return frexpd$1(val, exp);
}

#endif // _yIO_HAS_DEC$1

~) m4_dnl m4_applyforeachdefine;

