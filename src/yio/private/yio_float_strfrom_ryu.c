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
#include "yio/yio_error.h"
#include "yio_float_strfrom_ryu.h"
#include "yio_float_strfrom_custom.h"
#include "yio_res.h"
#include "yio_float.h"
#include "private.h"

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

#ifndef _yIO_USE_STRFROM_RYU
#error _yIO_USE_STRFROM_RYU
#endif
#if _yIO_USE_STRFROM_RYU

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

#ifndef _yIO_HAS_INT128
#error  _yIO_HAS_INT128
#endif
#if _yIO_HAS_INT128 && _yIO_HAS_FLOATl

// https://github.com/ulfjack/ryu/blob/master/ryu/ryu_generic_128.h
struct floating_decimal_128 {
  __uint128_t mantissa;
  int32_t exponent;
  bool sign;
};
struct floating_decimal_128 long_double_to_fd128(long double d);
int generic_to_chars(const struct floating_decimal_128 v, char* const result);

int _yIO_float_astrfrom_ryul(char ** const resultp, size_t * const lengthp,
		const int precision0, const char spec, _yIO_FLOATl val) {
	if (spec == 'a' || spec == 'A') {
		return _yIO_float_astrfrom_customl(resultp, lengthp, precision0, spec, val);
	}
	if (!(
			(spec == 'e' || spec == 'E') &&
			precision0 < 0
	)) {
		return YIO_ERROR_ENOSYS;
	}
	// https://github.com/ulfjack/ryu/blob/master/ryu/ryu_generic_128.h#L65
	*resultp = malloc(53);
	if (*resultp == NULL) {
		return YIO_ERROR_ENOMEM;
	}
	*lengthp = generic_to_chars(long_double_to_fd128(val), *resultp);
	if (spec == 'e') {
		*strchr(*resultp, 'E') = 'e';
	}
	return 0;
}

#endif

{% call j_FOREACHAPPLY(["f", "d"]) %}
#line

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

static inline
int _yIO_float_astrfrom_ryu$1_in(char ** const resultp, size_t * const lengthp,
		const int precision0, const char spec, _yIO_FLOAT$1 val) {
	// https://github.com/ulfjack/ryu/issues/197
	const size_t minsize = 2000;
	const int precision = precision0 < 0 ? 6 : precision0;
	const size_t toalloc = minsize + precision + 1;
	int len = 0;
	char *const res = malloc(toalloc);
	if (res == NULL) {
		*resultp = NULL;
		*lengthp = 0;
		return YIO_ERROR_ENOMEM;
	}
	//
	if (spec == 'g' || spec == 'G') {
		if (precision0 >= 0) {
			goto ERROR_ENOSYS;
		}
		len = d2s_buffered_n(val, res);
		exp_to_upper(res, spec == 'G');
	} else if (spec == 'f' || spec == 'F') {
		len = d2fixed_buffered_n(val, precision, res);
	} else if (spec == 'e' || spec == 'E') {
		len = d2exp_buffered_n(val, precision, res);
		exp_to_upper(res, spec == 'E');
	} else {
		goto ERROR_ENOSYS;
	}
	*resultp = res;
	*lengthp = len;
	return 0;
ERROR_ENOSYS:
	free(res);
	*resultp = NULL;
	*lengthp = 0;
	return YIO_ERROR_ENOSYS;
}

int _yIO_float_astrfrom_ryu$1(char ** const resultp, size_t * const lengthp,
		const int precision, const char spec, _yIO_FLOAT$1 val) {
	if (spec == 'a' || spec == 'A') {
		return _yIO_float_astrfrom_custom$1(resultp, lengthp, precision, spec, val);
	}
	return _yIO_float_astrfrom_ryu$1_in(resultp, lengthp, precision, spec, val);
}

#endif
{% endcall %}
#endif
