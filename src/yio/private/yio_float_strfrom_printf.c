/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE
#include "yio_float_strfrom_printf.h"
#include "yio_float.h"
#include "yio_res.h"
#include "private.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _yIO_USE_STRFROM_PRINTF
#error _yIO_USE_STRFROM_PRINTF
#endif
#if _yIO_USE_STRFROM_PRINTF

static inline
void _yIO_create_format_string_generic(char *fmt, size_t fmtsize,
		int precision, char spec, const char *pri, size_t prisize) {
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision >= 0) {
		*fmtpnt++ = '.';
		const int len = ysprint(fmtpnt, INT_MAX, precision);
		(void)len;
		assert(len > 0);
		assert((size_t)len < fmtsize - 2);
		fmtpnt += len;
	}
	memcpy(fmtpnt, pri, prisize);
	fmtpnt += prisize;
	*fmtpnt++ = spec;
	*fmtpnt++ = '\0';
	assert(fmtpnt <= fmt + fmtsize);
}

{% call j_FOREACHAPPLY(["f", "d", "l", "d32", "d64", "d128"]) %}
#line

#ifndef _yIO_has_float_printf$1
#error  _yIO_has_float_printf$1
#endif
#if _yIO_has_float_printf$1

#define FMT_SIZE$1 ( \
		\
		sizeof("%") - 1 + \
		sizeof(".") - 1 + \
		_yIO_INT_STRLEN_BOUND() + \
		sizeof(_yIO_FLOAT_PRI$1) - 1 + \
		sizeof(char) + \
		1 \
)

static inline
void _yIO_create_format_string$1(char fmt[FMT_SIZE$1], int precision, char spec) {
	_yIO_create_format_string_generic(fmt, FMT_SIZE$1,
			precision, spec, _yIO_FLOAT_PRI$1, sizeof(_yIO_FLOAT_PRI$1) - 1);
}

int _yIO_float_astrfrom_printf$1(_yIO_res *v, int precision, char spec, _yIO_FLOAT$1 val) {
	char fmt[FMT_SIZE$1];
	_yIO_create_format_string$1(fmt, precision, spec);
	assert(_yIO_res_size(v) < INT_MAX);
	const int len = snprintf(_yIO_res_data(v), _yIO_res_size(v), fmt, val);
	assert(len >= 0);
	if ((size_t)len < _yIO_res_size(v)) {
		_yIO_res_set_used(v, len);
	} else {
		int err = _yIO_res_resize2(v, len + 1, len);
		if (err) return err;

		const int len2 = snprintf(_yIO_res_data(v), _yIO_res_size(v), fmt, val);
		(void)len2;
		assert(len2 >= 0);
		assert(len2 == len);
		assert((size_t)len2 == _yIO_res_used(v));
	}
	return 0;
}

#endif

{% endcall %}
#endif
