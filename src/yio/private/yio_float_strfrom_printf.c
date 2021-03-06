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

#ifndef YIO_USE_STRFROM_PRINTF
#error YIO_USE_STRFROM_PRINTF
#endif
#if YIO_USE_STRFROM_PRINTF

static inline
void YYIO_create_format_string_generic(char *restrict fmt, size_t fmtsize,
		int precision, char spec, const char *restrict pri, size_t prisize) {
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

#ifndef YYIO_has_float_printf$1
#error  YYIO_has_float_printf$1
#endif
#if YYIO_has_float_printf$1

#define FMT_SIZE$1 ( \
		\
		sizeof("%") - 1 + \
		sizeof(".") - 1 + \
		YYIO_INT_STRLEN_BOUND() + \
		sizeof(YYIO_FLOAT_PRI$1) - 1 + \
		sizeof(char) + \
		1 \
)

static inline
void YYIO_create_format_string$1(char *restrict fmt, int precision, char spec) {
	YYIO_create_format_string_generic(fmt, FMT_SIZE$1,
			precision, spec, YYIO_FLOAT_PRI$1, sizeof(YYIO_FLOAT_PRI$1) - 1);
}

int YYIO_float_astrfrom_printf$1(YYIO_res *v, int precision, char spec, YYIO_FLOAT$1 val) {
	char fmt[FMT_SIZE$1];
	YYIO_create_format_string$1(fmt, precision, spec);
	assert(YYIO_res_size(v) < INT_MAX);
	const int len = snprintf(YYIO_res_data(v), YYIO_res_size(v), fmt, val);
	assert(len >= 0);
	if ((size_t)len >= YYIO_res_size(v)) {
		int err = YYIO_res_reserve(v, len + 1);
		if (err) return err;
		const int len2 = snprintf(YYIO_res_data(v), YYIO_res_size(v), fmt, val);
		(void)len2;
		assert(len2 >= 0);
		assert(len2 == len);
	}
	YYIO_res_set_used(v, len);
	return 0;
}

#endif

{% endcall %}
#endif
