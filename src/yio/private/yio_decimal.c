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
#include "private.h"
#include <math.h>

m4_applyforeachdefine(«(
	(d32),
	(d64),
	(d128),
)», m4_syncline()«

#if _yIO_HAS_FLOAT$1

#ifndef __STDC_DEC_FP__
// Forward declaration so we get compile time errors.
// If you see link errors, link with an implementation of decimal-library.
_yIO_FLOAT$1 floor$1(_yIO_FLOAT$1 x);
_yIO_FLOAT$1 log2$1(_yIO_FLOAT$1 x);
_yIO_FLOAT$1 exp2$1(_yIO_FLOAT$1 x);
_yIO_FLOAT$1 frexp$1(_yIO_FLOAT$1 x, int *);
#endif // __STDC_DEC_FP__

_yIO_FLOAT$1 _yIO_frexp2$1(_yIO_FLOAT$1 val, int *exp) {
	if (val == _yIO_FLOAT_C$1(0.0)) {
		*exp = _yIO_FLOAT_C$1(0.0);
	} else {
		const int tmp = 1 + floor$1(log2$1(val));
		*exp = tmp;
		val *= exp2$1(-tmp);
		if (val < _yIO_FLOAT_C$1(0.5)) {
			val = _yIO_FLOAT_C$1(0.5);
		} else if (val > _yIO_FLOAT_C$1(1.0)) {
			val = _yIO_FLOAT_C$1(1.0);
		}
	}
	return val;
}

_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp) {
	return frexp$1(val, exp);
}

#endif

»)

