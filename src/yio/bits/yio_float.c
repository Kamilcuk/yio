/**
 * @file yio_float.c
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
m4_config()
#include "yio_float.h"
#include <math.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

/* suffix $1 ---------------------------------------------------- */

_yIO_FLOAT$1 _yIO_frexp2$1(_yIO_FLOAT$1 val, int *exp) {
	return frexp$1(val, exp);
}

_yIO_FLOAT$1 _yIO_frexp10$1(_yIO_FLOAT$1 val, int *exp) {
	if (val == _yIO_FLOAT_C$1(0.0)) {
		*exp = _yIO_FLOAT_C$1(0.0);
	} else {
		const int tmp = 1 + floor$1(log10$1(val));
		*exp = tmp;
		val = (val) * exp10$1(-tmp);
		if (val < _yIO_FLOAT_C$1(0.1)) {
			val = _yIO_FLOAT_C$1(0.1);
		} else if (val > _yIO_FLOAT_C$1(1.0)) {
			val = _yIO_FLOAT_C$1(1.0);
		}
	}
	return val;
}

~) m4_dnl m4_applyforeachdefine

