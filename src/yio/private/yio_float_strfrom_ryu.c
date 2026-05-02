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
	const size_t minsize = 2000;
	const size_t precision = yio_precision_get_default(precision0, 6);
	const size_t toalloc = minsize + precision + 1;
	int err = YYIO_string_reserve(res, toalloc);
	if (err) return err;
	int len = 0;
	char *const buf = YYIO_string_data(res);
	//
	if (spec == 'g' || spec == 'G') {
		if (precision0 != 0) {
			return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
		}
		len = d2s_buffered_n(val, buf);
		exp_to_upper(buf, spec == 'G');
	} else if (spec == 'f' || spec == 'F') {
		len = d2fixed_buffered_n(val, precision, buf);
	} else if (spec == 'e' || spec == 'E') {
		len = d2exp_buffered_n(val, precision, buf);
		exp_to_upper(buf, spec == 'E');
	} else {
		return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
	}
	if (len <= 0) {
		return YYIO_RYU_FALLBACK$1(res, precision0, spec, val);
	}
	YYIO_string_set_used(res, len);
	//
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
