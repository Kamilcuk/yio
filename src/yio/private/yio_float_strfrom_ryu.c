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
#include "yio_res.h"

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
#error YIO_USE_STRFROM_RYU
#endif
#if YIO_USE_STRFROM_RYU

/* ------------------------------------------------------------------------- */

// https://github.com/ulfjack/ryu/blob/master/ryu/ryu.h
int d2s_buffered_n(double f, char* result);
int d2fixed_buffered_n(double d, uint32_t precision, char* result);
int d2exp_buffered_n(double d, uint32_t precision, char* result);

/* ------------------------------------------------------------------------- */

static inline
void exp_to_upper(char *str, bool doit) {
	if (!doit) return;
	char *tmp = strchr(str, 'e');
	if (tmp) {
		*tmp = 'E';
	}
}

/* ------------------------------------------------------------------------- */

#ifndef YYIO_HAS_INT128
#error  YYIO_HAS_INT128
#endif
#if YYIO_HAS_INT128 && YIO_HAS_FLOATl

// copied from https://github.com/ulfjack/ryu/blob/master/ryu/ryu_generic_128.h
// depends on ABI
struct floating_decimal_128 {
  __uint128_t mantissa;
  int32_t exponent;
  bool sign;
};
struct floating_decimal_128 long_double_to_fd128(long double d);
int generic_to_chars(struct floating_decimal_128 v, char *result);

int YYIO_float_astrfrom_ryul(YYIO_res *res, const int precision0, const char spec, YYIO_FLOATl val) {
	if (spec == 'a' || spec == 'A') {
		return YYIO_float_astrfrom_customl(res, precision0, spec, val);
	}
	if (!(
			(spec == 'e' || spec == 'E') &&
			precision0 < 0
	)) {
		return YIO_ERROR_ENOSYS;
	}
	// https://github.com/ulfjack/ryu/blob/master/ryu/ryu_generic_128.h#L65
	const size_t maximal_char_buffer_requirement_for_ryu = 53;
	int err = YYIO_res_reserve(res, maximal_char_buffer_requirement_for_ryu);
	if (err) return err;
	int lengthp = generic_to_chars(long_double_to_fd128(val), YYIO_res_begin(res));
	if (spec == 'e') {
		*strchr(YYIO_res_begin(res), 'E') = 'e';
	}
	YYIO_res_set_used(res, lengthp);
	return 0;
}

#endif

{% call j_FOREACHAPPLY(["f", "d"]) %}
#line

#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1

static inline
int YYIO_float_astrfrom_ryu$1_in(YYIO_res *res, const int precision0, const char spec, YYIO_FLOAT$1 val) {
	// https://github.com/ulfjack/ryu/issues/197
	const size_t minsize = 2000;
	const int precision = precision0 < 0 ? 6 : precision0;
	const size_t toalloc = minsize + precision + 1;
	int err = YYIO_res_reserve(res, toalloc);
	if (err) return err;
	int len = 0;
	char *const buf = YYIO_res_begin(res);
	//
	if (spec == 'g' || spec == 'G') {
		if (precision0 >= 0) {
			goto ERROR_ENOSYS;
		}
		len = d2s_buffered_n(val, buf);
		exp_to_upper(buf, spec == 'G');
	} else if (spec == 'f' || spec == 'F') {
		len = d2fixed_buffered_n(val, precision, buf);
	} else if (spec == 'e' || spec == 'E') {
		len = d2exp_buffered_n(val, precision, buf);
		exp_to_upper(buf, spec == 'E');
	} else {
		goto ERROR_ENOSYS;
	}
	YYIO_res_set_used(res, len);
	//
	return 0;
ERROR_ENOSYS:
	return YIO_ERROR_ENOSYS;
}

int YYIO_float_astrfrom_ryu$1(YYIO_res *res, const int precision, const char spec, YYIO_FLOAT$1 val) {
	if (spec == 'a' || spec == 'A') {
		return YYIO_float_astrfrom_custom$1(res, precision, spec, val);
	}
	return YYIO_float_astrfrom_ryu$1_in(res, precision, spec, val);
}

#endif
{% endcall %}
#endif
