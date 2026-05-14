/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#define _XOPEN_SOURCE  700
#define _POSIX_C_SOURCE  200809L
#define _BSD_SOURCE  1
#define _SVID_SOURCE  1
#define _DEFAULT_SOURCE  1
#define __STDC_WANT_LIB_EXT2__  1
#define __STDC_WANT_DEC_FP__  1
#define __STDC_WANT_IEC_60559_EXT__  1
#define __STDC_WANT_IEC_60559_DFP_EXT__  1
#define __STDC_WANT_IEC_60559_BFP_EXT__  1
#define __STDC_WANT_IEC_60559_FUNCS_EXT__  1
#define __STDC_WANT_IEC_60559_TYPES_EXT__  1
#include "yio_float_strfrom_ryu.h"
#include "private.h"
#include "yio/yio_error.h"
#include "yio_float.h"
#include "yio_float_strfrom_naive.h"
#include "yio_string.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YIO_USE_STRFROM_RYU
#error YIO_USE_STRFROM_RYU not defined
#endif

#if YIO_USE_STRFROM_RYU

/* ------------------------------------------------------------------------- */

// https://github.com/ulfjack/ryu/blob/master/ryu/ryu.h
int d2s_buffered_n(double f, char* result);
int d2fixed_buffered_n(double d, uint32_t precision, char* result);
int d2exp_buffered_n(double d, uint32_t precision, char* result);

/* ------------------------------------------------------------------------- */

static inline void exp_to_upper(YIO_string *t) {
	char *tmp = memchr(YIO_string_data(t), 'e', YIO_string_len(t));
	if (tmp) *tmp = 'E';
}

/* ------------------------------------------------------------------------- */

#if YIO_HAS_INT128
#include "ryu/ryu_generic_128.h"
#endif

/* Use the actual maximums for Ryu's shortest representation */
#define YIO_RYU_SHORTEST_MAX_B32 15
#define YIO_RYU_SHORTEST_MAX_B64 25

{% call(V) j_FOREACHAPPLY(j_FLOATREPRS) %}
#line
#ifdef YIO_FLOAT_RP_$1

{% if V.1 in ["B32", "B64"] %}
int YIO_float_astrfrom_ryu_$1(YIO_string *res, int precision, char spec, YIO_FLOAT_RP_$1 val) {
	if (spec == 'a' || spec == 'A') {
		return YIO_float_astrfrom_naive_$1(res, precision, spec, val);
	}
	if (!isfinite(val)) {
		return YIO_float_astrfrom_naive_$1(res, precision, spec, val);
	}
	// https://github.com/ulfjack/ryu/issues/197
	const size_t precision_def = precision < 0 ? 6 : (size_t)precision;
	int err;
	int len = -1;
	if (spec == 'g' || spec == 'G') {
		if (precision >= 0) return YIO_float_astrfrom_naive_$1(res, precision, spec, val);
		err = YIO_string_reserve(res, YIO_RYU_SHORTEST_MAX_$1);
    if (err) return err;
    double dval = (double)val;
    double abs_val = (dval < 0) ? -dval : dval;
    if (abs_val >= 0.0001 && abs_val < 1000000.0) {
      len = d2fixed_buffered_n((double)val, 6, YIO_string_data(res));
      if (len > 0) {
      	YIO_string_set_used(res, len);
      	YIO_string_remove_trailing_zeros_and_dot(res);
      	return 0;
      }
    } else {
      len = d2s_buffered_n((double)val, YIO_string_data(res));
			if (len > 0) {
				YIO_string_set_used(res, len);
				if (spec == 'G') exp_to_upper(res);
			}
    }
	} else if (spec == 'f' || spec == 'F') {
    const size_t to_reserve = YIO_MAX_10_EXP_RP_$1 + 3 + precision_def;
  	err = YIO_string_reserve(res, to_reserve);
  	if (err) return err;
		len = d2fixed_buffered_n((double)val, precision_def, YIO_string_data(res));
		if (len > 0) {
			YIO_string_set_used(res, len);
		}
	} else if (spec == 'e' || spec == 'E') {
    const size_t overhead = YIO_MAX_10_EXP_RP_$1 > 99 ? 9 : 8;
    const size_t to_reserve = overhead + precision_def;
    err = YIO_string_reserve(res, to_reserve);
    if (err) return err;
		len = d2exp_buffered_n((double)val, precision_def, YIO_string_data(res));
		if (len > 0) {
			YIO_string_set_used(res, len);
			if (spec == 'E') exp_to_upper(res);
		}
	} else {
		return YIO_float_astrfrom_naive_$1(res, precision, spec, val);
	}
	assert(len > 0);
	return 0;
}

{% elif V.1 in ["B80", "B128"] %}
#if YIO_HAS_INT128
int YIO_float_astrfrom_ryu_$1(YIO_string *res, int precision0, char spec, YIO_FLOAT_RP_$1 val) {
	if (!isfinite(val)) {
		return YIO_float_astrfrom_naive_$1(res, precision0, spec, val);
	}
	if (spec == 'a' || spec == 'A') {
		return YIO_float_astrfrom_naive_$1(res, precision0, spec, val);
	}
	if (!( (spec == 'e' || spec == 'E') && precision0 < 0 )) {
		return YIO_float_astrfrom_naive_$1(res, precision0, spec, val);
	}
	const size_t maximal_char_buffer_requirement_for_ryu = 53;
	int err = YIO_string_reserve(res, maximal_char_buffer_requirement_for_ryu);
	if (err) return err;
	int lengthp = generic_to_chars(long_double_to_fd128(val), YIO_string_data(res));
	if (lengthp <= 0) {
		return YIO_float_astrfrom_naive_$1(res, precision0, spec, val);
	}
	if (spec == 'e') {
		char *const e = strchr(YIO_string_data(res), 'E');
		if (e) *e = 'e';
	}
	YIO_string_set_used(res, lengthp);
	return 0;
}
#else
int YIO_float_astrfrom_ryu_$1(YIO_string *res, int precision, char spec0, YIO_FLOAT_RP_$1 val) {
	return YIO_float_astrfrom_naive_$1(res, precision, spec0, val);
}
#endif

{% else %}
int YIO_float_astrfrom_ryu_$1(YIO_string *res, int precision, char spec0, YIO_FLOAT_RP_$1 val) {
	return YIO_float_astrfrom_naive_$1(res, precision, spec0, val);
}
{% endif %}

#endif
{% endcall %}

#endif
