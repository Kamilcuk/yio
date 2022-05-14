/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <float.h>
#define _GNU_SOURCE
#include "yio_float.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

{% call j_FOREACHAPPLY(["f", "d", "l", "f16", "f32", "f64", "f128", "f32x", "f64x", "f128x"]) %}

/* suffix $1 ---------------------------------------------------- */

#ifndef _yIO_MUSL_BROKEN_EXP10
#error  _yIO_MUSL_BROKEN_EXP10
#endif

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp) {
	const int tmp = val == 0 ? 0 : (int)(
			_yIO_FLOAT_C$1(1.0) + _yIO_floor$1(_yIO_log10$1(_yIO_fabs$1(val)))
	);
	*exp = tmp;
	_yIO_FLOAT$1 ex = _yIO_exp10$1( (_yIO_FLOAT$1)-tmp );
	const _yIO_FLOAT$1 minval = _yIO_FLOAT_C$1(0.1);
#if _yIO_MUSL_BROKEN_EXP10
	// Musl incorrectly implements exp10 for big numbers.
	if (ex == 0 && tmp > 10) {
		ex = minval;
		for (int a = tmp; --a && ex != 0; ) {
			ex *= minval;
		}
	}
#endif
	val *= ex;
	//printf("%Lg %d %Lg %Lg %Lg\n", (long double)exp10l(-tmp), errno, (long double)powl(10.0, -tmp), (long double)ex, (long double)val);
	//printf("%d %zu %d %d %d\n", DBL_MANT_DIG, sizeof(double), DBL_MAX_EXP, DBL_MAX_10_EXP, DBL_HAS_SUBNORM);
	if (val < minval) {
		val = minval;
	} else if (val >= _yIO_FLOAT_C$1(1.0)) {
		val = _yIO_nextafter$1(_yIO_FLOAT_C$1(1.0), 0);
	}
	return val;
}

#endif // _yIO_HAS_FLOAT$1

{% endcall %}
