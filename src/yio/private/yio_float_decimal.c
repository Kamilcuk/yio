/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include "yio_float.h"
#include "private.h"
#include <math.h>

{% call j_FOREACHAPPLY(["d32", "d64", "d128", "d32x", "d64x", "d128x"]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

// Forward declaration so we get compile time errors.
// If you see link errors, link with an implementation of decimal-library.
YYIO_FLOAT$1 floor$1(YYIO_FLOAT$1 x);
YYIO_FLOAT$1 log2$1(YYIO_FLOAT$1 x);
YYIO_FLOAT$1 exp2$1(YYIO_FLOAT$1 x);
YYIO_FLOAT$1 frexp$1(YYIO_FLOAT$1 x, int *);

YYIO_FLOAT$1 YYIO_frexp2$1(YYIO_FLOAT$1 val, int *exp) {
	if (val == YYIO_FLOAT_C$1(0.0)) {
		*exp = YYIO_FLOAT_C$1(0.0);
	} else {
		const int tmp = 1 + floor$1(log2$1(val));
		*exp = tmp;
		val *= exp2$1(-tmp);
		if (val < YYIO_FLOAT_C$1(0.5)) {
			val = YYIO_FLOAT_C$1(0.5);
		} else if (val > YYIO_FLOAT_C$1(1.0)) {
			val = YYIO_FLOAT_C$1(1.0);
		}
	}
	return val;
}

#endif

{% endcall %}

