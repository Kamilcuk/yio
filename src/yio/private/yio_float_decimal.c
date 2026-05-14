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
#ifdef YIO_FLOAT_RP_{{R.name}}

#define TYPE     YIO_FLOAT_RP_{{R.name}}
#define FLOOR    YIO_floor_RP_{{R.name}}
#define LOG2     YIO_log2_RP_{{R.name}}
#define EXP2     YIO_exp2_RP_{{R.name}}
#define FC(x)    YIO_FLOAT_C_RP_{{R.name}}(x)

TYPE YIO_frexp2_RP_{{R.name}}_IMPL(TYPE val, int *exp) {
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
