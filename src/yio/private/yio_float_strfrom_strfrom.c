/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#define __STDC_WANT_DEC_FP__ 1
#define __STDC_WANT_IEC_60559_BFP_EXT__  1
#include "yio_float_strfrom_strfrom.h"
#include "yio_float.h"
#include "yio_res.h"
#include "private.h"
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define FMT_SIZE ( \
		\
		sizeof("%") - 1 + \
		sizeof(".") - 1 + \
		_yIO_INT_STRLEN_BOUND() + \
		sizeof(char) + \
		1 \
)

static const unsigned fmt_size = FMT_SIZE;

static inline
void _yIO_float_astrfrom_strfrom_create_format_string(char *fmt, int precision, char spec) {
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision >= 0) {
		*fmtpnt++ = '.';
		const int len = ysprint(fmtpnt, INT_MAX, precision);
		(void)len;
		assert(len > 0);
		assert((size_t)len < fmt_size - 2);
		fmtpnt += len;
	}
	*fmtpnt++ = spec;
	*fmtpnt++ = '\0';
	assert(fmtpnt <= &fmt[fmt_size]);
}

{% call j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1
#ifndef _yIO_HAS_strfrom$1
#error  _yIO_HAS_strfrom$1
#endif

// In case it's not defined in standard headers, so that we get a link time error.
extern int strfrom$1(char *str, size_t n, const char *format, _yIO_FLOAT$1 fp);

int _yIO_float_astrfrom_strfrom$1(_yIO_res *v, int precision, char spec, _yIO_FLOAT$1 val) {
	// create format string
	char fmt[FMT_SIZE];
	_yIO_float_astrfrom_strfrom_create_format_string(fmt, precision, spec);
	// get length
	assert(_yIO_res_size(v) < INT_MAX);
	const int len = strfrom$1(_yIO_res_data(v), _yIO_res_size(v), fmt, val);
	if (len <= 0) {
		// this is not possible
		return _yIO_ERROR(YIO_ERROR_STRFROM, "strfrom returned -1");
	}
	if ((size_t)len < _yIO_res_size(v)) {
		_yIO_res_set_used(v, len);
	} else {
		const int err = _yIO_res_resize2(v, len + 1, len);
		if (err) return err;
		const int len2 = strfrom$1(_yIO_res_data(v), _yIO_res_size(v), fmt, val);
		(void)len2;
		assert((size_t)len2 == _yIO_res_used(v));
		assert(len2 == len);
	}
	return 0;
}

#endif

{% endcall %}
