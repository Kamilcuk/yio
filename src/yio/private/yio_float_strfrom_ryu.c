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
#include "yio_float_strfrom_custom.h"
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

#include "yio_float_strfrom_custom.h"

#ifndef YYIO_RYU_FALLBACKf
#define YYIO_RYU_FALLBACKf  YYIO_float_astrfrom_customf
#endif

#ifndef YYIO_RYU_FALLBACKd
#define YYIO_RYU_FALLBACKd  YYIO_float_astrfrom_customd
#endif

#ifndef YYIO_RYU_FALLBACKl
#define YYIO_RYU_FALLBACKl  YYIO_float_astrfrom_customl
#endif

#ifndef YIO_USE_STRFROM_RYU
#error YIO_USE_STRFROM_RYU not defined
#endif
#ifndef YIO_HAS_FLOATf
#error YIO_HAS_FLOATf not defined
#endif
#ifndef YIO_HAS_FLOATd
#error YIO_HAS_FLOATd not defined
#endif
#if YIO_USE_STRFROM_RYU && YIO_HAS_FLOATf && YIO_HAS_FLOATd

/* ------------------------------------------------------------------------- */

// https://github.com/ulfjack/ryu/blob/master/ryu/ryu.h
int d2s_buffered_n(double f, char* result);
int d2fixed_buffered_n(double d, uint32_t precision, char* result);
int d2exp_buffered_n(double d, uint32_t precision, char* result);

/* ------------------------------------------------------------------------- */

static inline void exp_to_upper(YYIO_string *t) {
	char *tmp = memchr(YYIO_string_data(t), 'e', YYIO_string_len(t));
	if (tmp) *tmp = 'E';
}

/* ------------------------------------------------------------------------- */

#ifndef YYIO_HAS_INT128
#error  YYIO_HAS_INT128
#endif
#if YYIO_HAS_INT128 && YIO_HAS_FLOATl

#include "ryu/ryu_generic_128.h"

int YYIO_float_astrfrom_ryul(YYIO_string *res, int precision0, char spec, YYIO_FLOATl val) {
	if (!isfinite(val)) {
		return YYIO_RYU_FALLBACKl(res, precision0, spec, val);
	}
	if (spec == 'a' || spec == 'A') {
		return YYIO_RYU_FALLBACKl(res, precision0, spec, val);
	}
	if (!(
			(spec == 'e' || spec == 'E') &&
			precision0 < 0
	)) {
		return YYIO_RYU_FALLBACKl(res, precision0, spec, val);
	}
	// https://github.com/ulfjack/ryu/blob/master/ryu/ryu_generic_128.h#L65
	const size_t maximal_char_buffer_requirement_for_ryu = 53;
	int err = YYIO_string_reserve(res, maximal_char_buffer_requirement_for_ryu);
	if (err) return err;
	int lengthp = generic_to_chars(long_double_to_fd128(val), YYIO_string_data(res));
	if (lengthp <= 0) {
		return YYIO_RYU_FALLBACKl(res, precision0, spec, val);
	}
	if (spec == 'e') {
		char *const e = strchr(YYIO_string_data(res), 'E');
		if (e) *e = 'e';
	}
	YYIO_string_set_used(res, lengthp);
	return 0;
}

#endif

/* Use the actual maximums for Ryu's shortest representation */
#define YYIO_RYU_SHORTEST_MAX_f 15
#define YYIO_RYU_SHORTEST_MAX_d 25

static inline
bool YYIO_string_remove_trailing_zeros_and_comma(YYIO_string *t) {
	bool fractional_part_removed = false;
	const size_t len = YYIO_string_len(t);
	if (len == 0) return false;
	char * const data = YYIO_string_data(t);
	char *p = data + len - 1;
	// there is dot, so the following loop will always stop
	while (p != data && *p == '0') {
		--p;
	}
	assert(YYIO_isxdigit(*p) || *p == '.');
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	YYIO_string_set_used(t, (size_t)(p - data));
	return fractional_part_removed;
}

{% call j_FOREACHAPPLY(["f", "d"]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

static inline
int YYIO_float_astrfrom_ryu$1_in(YYIO_string *res, int precision0, char spec, YYIO_FLOAT$1 val) {
	if (!isfinite(val)) {
		return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
	}
	// https://github.com/ulfjack/ryu/issues/197
	const size_t precision = yio_precision_get_default(precision0, 6);
	int err;
	int len = -1;
	//
	if (spec == 'g' || spec == 'G') {
		if (precision0 != 0) return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
		err = YYIO_string_reserve(res, YYIO_RYU_SHORTEST_MAX_$1);
    if (err) return err;
		// Rational check: Use fixed-point for "human-readable" ranges
    // Standard %g uses fixed-point if -4 <= exponent < precision
    YYIO_FLOAT$1 dval = (double)val;
    YYIO_FLOAT$1 abs_val = (dval < 0) ? -dval : dval;
    if (abs_val >= 0.0001 && abs_val < 1000000.0) {
      // Use fixed-point logic but you MUST strip trailing zeros
      len = d2fixed_buffered_n(val, 6, YYIO_string_data(res));
      if (len > 0) {
      	YYIO_string_set_used(res, len);
      	YYIO_string_remove_trailing_zeros_and_comma(res);
      	return 0;
      }
    } else {
      len = d2s_buffered_n(val, YYIO_string_data(res));
			if (len > 0) {
				YYIO_string_set_used(res, len);
				if (spec == 'G') exp_to_upper(res);
			}
    }
	} else if (spec == 'f' || spec == 'F') {
		/*
     * Max integer digits + sign + dot + precision + null
     * For double: 308 + 3 + precision
     * For float:  38 + 3 + precision
     */
    const size_t to_reserve = YYIO_MAX_10_EXP$1 + 3 + precision;
  	err = YYIO_string_reserve(res, to_reserve);
  	if (err) return err;
		len = d2fixed_buffered_n(val, precision, YYIO_string_data(res));
		if (len > 0) {
			YYIO_string_set_used(res, len);
		}
	} else if (spec == 'e' || spec == 'E') {
		/* Overhead: sign(1), digit(1), dot(1), 'e'(1), exp_sign(1), exp_digits(2 or 3), null(1) */
    const size_t overhead = YYIO_MAX_10_EXP$1 > 99 ? 9 : 8;
    const size_t to_reserve = overhead + precision;
    err = YYIO_string_reserve(res, to_reserve);
    if (err) return err;
		len = d2exp_buffered_n(val, precision, YYIO_string_data(res));
		if (len > 0) {
			YYIO_string_set_used(res, len);
			if (spec == 'E') exp_to_upper(res);
		}
	} else {
		return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
	}
	assert(len > 0);
	return 0;
}

int YYIO_float_astrfrom_ryu$1(YYIO_string *res, int precision, char spec, YYIO_FLOAT$1 val) {
	if (spec == 'a' || spec == 'A') {
		return YYIO_RYU_FALLBACK$1(res, precision, spec, val);
	}
	return YYIO_float_astrfrom_ryu$1_in(res, precision, spec, val);
}

#endif
{% endcall %}
#endif
