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
#include "yio_string.h"
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
void YYIO_float_astrfrom_strfrom_create_format_string(char *fmt, int precision0, char spec) {
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision0 != 0) {
		*fmtpnt++ = '.';
		const size_t precision = yio_precision_get_default(precision0, 0);
		const int len = yio_snstream(fmtpnt, INT_MAX, precision);
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
#ifdef YYIO_FLOAT_REPR_$1
#if YYIO_HAS_strfrom_RC_$1

int YYIO_float_astrfrom_strfrom_$1(YYIO_string *v, int precision0, char spec, YYIO_FLOAT_REPR_$1 val) {
	// create format string
	char fmt[FMT_SIZE];
	YYIO_float_astrfrom_strfrom_create_format_string(fmt, precision0, spec);
	// get length
	assert(YYIO_string_capacity(v) < INT_MAX);
	const int len = YYIO_strfrom_RC_$1(YYIO_string_data(v), YYIO_string_capacity(v), fmt, val);
	if (len <= 0) {
		// this is not possible
		return YYIO_ERROR(YIO_ERROR_STRFROM, "strfrom returned -1");
	}
	if ((size_t)len >= YYIO_string_capacity(v)) {
		const int err = YYIO_string_reserve(v, len + 1);
		if (err) return err;
		const int len2 = YYIO_strfrom_RC_$1(YYIO_string_data(v), YYIO_string_capacity(v), fmt, val);
		(void)len2;
		assert(len2 > 0);
		assert(len2 == len);
	}
	YYIO_string_set_used(v, len);
	return 0;
}

#endif
#endif

{% endcall %}
