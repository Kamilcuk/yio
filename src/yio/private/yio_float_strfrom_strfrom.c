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
#define __STDC_WANT_IEC_60559_DFP_EXT__  1
#include "yio_float_strfrom_strfrom.h"
#include "private.h"
#include "yio_float.h"
#include "yio_buf.h"
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
		YIO_INT_STRLEN_BOUND() + \
		sizeof(char) + \
		1 \
)

static const unsigned fmt_size = FMT_SIZE;

static inline
void YIO_float_astrfrom_strfrom_create_format_string(char *fmt, int precision0, char spec) {
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision0 >= 0) {
		*fmtpnt++ = '.';
		const int len = yio_snstream(fmtpnt, INT_MAX, precision0);
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

{% call(V) j_FOREACHAPPLY(j_FLOATREPRS) %}
#line
#if YIO_has_float_astrfrom_strfrom_$1

int YIO_float_astrfrom_strfrom_$1(YIO_buf *v, int precision0, char spec, YIO_FLOAT_RP_$1 val) {
	// create format string
	char fmt[FMT_SIZE];
	YIO_float_astrfrom_strfrom_create_format_string(fmt, precision0, spec);
	// get length
	assert(YIO_buf_capacity(v) < INT_MAX);
	const int len = YIO_strfrom_RP_$1(YIO_buf_data(v), YIO_buf_capacity(v), fmt, val);
	if (len <= 0) {
		return YIO_ERROR(YIO_ERROR_STRFROM, "strfrom returned zero or negative");
	}
	if ((size_t)len >= YIO_buf_capacity(v)) {
		const int err = YIO_buf_reserve(v, len + 1);
		if (err) return err;
		const int len2 = YIO_strfrom_RP_$1(YIO_buf_data(v), YIO_buf_capacity(v), fmt, val);
		if (len2 != len) {
			return YIO_ERROR_STRFROM;
		}
	}
	YIO_buf_set_used(v, len);
	return 0;
}

#endif

{% endcall %}
