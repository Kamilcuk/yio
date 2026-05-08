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

{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_RP_{{R.name}}

#ifndef YYIO_MUSL_BROKEN_EXP10
#error  YYIO_MUSL_BROKEN_EXP10
#endif

#define TYPE     YYIO_FLOAT_RP_{{R.name}}
#define FLOOR    YYIO_floor_RP_{{R.name}}
#define LOG10    YYIO_log10_RP_{{R.name}}
#define FABS     YYIO_fabs_RP_{{R.name}}
#define EXP10    YYIO_exp10_RP_{{R.name}}
#define NEXTAFTER YYIO_nextafter_RP_{{R.name}}
#define FC(x)    YYIO_FLOAT_C_RP_{{R.name}}(x)

TYPE YYIO_frexp10_RP_{{R.name}}_IMPL(TYPE val, int *exp) {
	if (val == FC(0.0)) {
		*exp = 0;
		return val;
	}
	const int tmp = (int)(
			FC(1.0) + FLOOR(LOG10(FABS(val)))
	);
	*exp = tmp;
	TYPE ex = EXP10( (TYPE)-( (long long)tmp ) );
	const TYPE minval = FC(0.1);
#if YYIO_MUSL_BROKEN_EXP10
	// Musl incorrectly implements exp10 for big numbers.
	if (ex == 0 && tmp > 10) {
		ex = minval;
		for (int a = tmp; --a && ex != 0; ) {
			ex *= minval;
		}
	}
#endif
	val *= ex;
	if (val < minval) {
		val = minval;
	} else if (val >= FC(1.0)) {
		val = NEXTAFTER(FC(1.0), 0);
	}
	return val;
}

#undef TYPE
#undef FLOOR
#undef LOG10
#undef FABS
#undef EXP10
#undef NEXTAFTER
#undef FC

#endif
{% endfor %}
