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

{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_REPR_{{R}}

#define TYPE     YYIO_FLOAT_REPR_{{R}}
#define FLOOR    YYIO_floor_RC_{{R}}
#define LOG2     YYIO_log2_RC_{{R}}
#define EXP2     YYIO_exp2_RC_{{R}}
#define FC(x)    YYIO_FLOAT_C_RC_{{R}}(x)

TYPE YYIO_frexp2_RC_{{R}}_IMPL(TYPE val, int *exp) {
	if (val == FC(0.0)) {
		*exp = 0;
	} else {
		const int tmp = (int)(FC(1.0) + FLOOR(LOG2(val)));
		*exp = tmp;
		val *= EXP2((TYPE)-( (long long)tmp ));
		if (val < FC(0.5)) {
			val = FC(0.5);
		} else if (val > FC(1.0)) {
			val = FC(1.0);
		}
	}
	return val;
}

#undef TYPE
#undef FLOOR
#undef LOG2
#undef EXP2
#undef FC

#endif
{% endfor %}
