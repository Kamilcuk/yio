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
#include "yio_string.h"
#include "private.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YIO_USE_STRFROM_PRINTF
#error YIO_USE_STRFROM_PRINTF
#endif
#if YIO_USE_STRFROM_PRINTF

static inline
void YYIO_create_format_string_generic(char *restrict fmt, size_t fmtsize,
		int precision0, char spec, const char *restrict pri, size_t prisize) {
	(void)fmtsize;
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision0 != 0) {
		*fmtpnt++ = '.';
		const size_t precision = yio_precision_get_default(precision0, 0);
		const int len = yio_snstream(fmtpnt, INT_MAX, precision);
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

{% call(V) j_FOREACHAPPLY(j_FLOATREPRS) %}
#line
#ifdef YYIO_FLOAT_REPR_$1

#define FMT_SIZE_$1 ( \
		\
		sizeof("%") - 1 + \
		sizeof(".") - 1 + \
		YYIO_INT_STRLEN_BOUND() + \
		sizeof(YYIO_FLOAT_PRI_RC_$1) - 1 + \
		sizeof(char) + \
		1 \
)

static inline
void YYIO_create_format_string_$1(char *restrict fmt, int precision0, char spec) {
	YYIO_create_format_string_generic(fmt, FMT_SIZE_$1,
			precision0, spec, YYIO_FLOAT_PRI_RC_$1, sizeof(YYIO_FLOAT_PRI_RC_$1) - 1);
}

int YYIO_float_astrfrom_printf_$1(YYIO_string *v, int precision0, char spec, YYIO_FLOAT_REPR_$1 val) {
	char fmt[FMT_SIZE_$1];
	YYIO_create_format_string_$1(fmt, precision0, spec);
	assert(YYIO_string_capacity(v) < INT_MAX);
	const int len = snprintf(YYIO_string_data(v), YYIO_string_capacity(v), fmt, (YYIO_FLOAT_PRINTF_TYPE_RC_$1)val);
	assert(len >= 0);
	if ((size_t)len >= YYIO_string_capacity(v)) {
		int err = YYIO_string_reserve(v, len + 1);
		if (err) return err;
		const int len2 = snprintf(YYIO_string_data(v), YYIO_string_capacity(v), fmt, (YYIO_FLOAT_PRINTF_TYPE_RC_$1)val);
		(void)len2;
		assert(len2 >= 0);
		assert(len2 == len);
	}
	YYIO_string_set_used(v, len);
	return 0;
}

#endif

{% endcall %}
#endif
