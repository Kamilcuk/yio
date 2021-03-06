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
#include "private.h"
#include "yio_float.h"
#include "yio_res.h"
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
		YYIO_INT_STRLEN_BOUND() + \
		sizeof(char) + \
		1 \
)

static const unsigned fmt_size = FMT_SIZE;

static inline
void YYIO_float_astrfrom_strfrom_create_format_string(char *fmt, int precision, char spec) {
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
	(void)fmt_size;
}

{% call j_FOREACHAPPLY(j_FLOATS) %}
#line
#ifndef YIO_HAS_FLOAT$1
#error  YIO_HAS_FLOAT$1
#endif
#if YIO_HAS_FLOAT$1
#ifndef YYIO_HAS_strfrom$1
#error  YYIO_HAS_strfrom$1
#endif

#if !YYIO_HAS_strfrom$1
// In case it's not defined in standard headers, so that we get a link time error.
extern int strfrom$1(char *str, size_t n, const char *format, YYIO_FLOAT$1 fp);
#endif

int YYIO_float_astrfrom_strfrom$1(YYIO_res *v, int precision, char spec, YYIO_FLOAT$1 val) {
	// create format string
	char fmt[FMT_SIZE];
	YYIO_float_astrfrom_strfrom_create_format_string(fmt, precision, spec);
	// get length
	assert(YYIO_res_size(v) < INT_MAX);
	const int len = strfrom$1(YYIO_res_data(v), YYIO_res_size(v), fmt, val);
	if (len <= 0) {
		// this is not possible
		return YYIO_ERROR(YIO_ERROR_STRFROM, "strfrom returned -1");
	}
	if ((size_t)len >= YYIO_res_size(v)) {
		const int err = YYIO_res_reserve(v, len + 1);
		if (err) return err;
		const int len2 = strfrom$1(YYIO_res_data(v), YYIO_res_size(v), fmt, val);
		(void)len2;
		assert(len2 > 0);
		assert(len2 == len);
	}
	YYIO_res_set_used(v, len);
	return 0;
}

#endif

{% endcall %}
