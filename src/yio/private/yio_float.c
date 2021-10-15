/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
#include "yio_float.h"
#include <math.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>

m4_applyforeachdefine(«(
			(f), (d), (l),
			(f16), (f32), (f64), (f128),
			(f32x), (f64x), (f128x),
)», m4_syncline(1)«m4_dnl;

/* suffix $1 ---------------------------------------------------- */

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp) {
	if (val == _yIO_FLOAT_C$1(0.0)) {
		*exp = _yIO_FLOAT_C$1(0.0);
	} else {
		const int tmp = _yIO_FLOAT_C$1(1.0) + _yIO_floor$1(_yIO_log10$1(val));
		*exp = tmp;
		val = (val) * _yIO_exp10$1(-tmp);
		if (val < _yIO_FLOAT_C$1(0.1)) {
			val = _yIO_FLOAT_C$1(0.1);
		} else if (val > _yIO_FLOAT_C$1(1.0)) {
			val = _yIO_FLOAT_C$1(1.0);
		}
	}
	return val;
}

#endif // _yIO_HAS_FLOAT$1

») m4_dnl m4_applyforeachdefine

