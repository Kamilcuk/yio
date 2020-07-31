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
#include "private.h"
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern int asprintf(char **strp, const char *fmt, ...);

static inline
void create_format_string_generic(char *fmt, size_t fmtsize,
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

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

#ifdef _yIO_HAS_FLOAT$1

#define FMT_SIZE$1 ( \
		\
		sizeof("%") - 1 + \
		sizeof(".") - 1 + \
		INT_STRLEN_BOUND(int) + \
		sizeof(_yIO_FLOAT_PRI$1) - 1 + \
		sizeof(char) + \
		1 \
)

static inline
void create_format_string$1(char fmt[FMT_SIZE$1], int precision, char spec) {
	create_format_string_generic(fmt, FMT_SIZE$1,
			precision, spec, _yIO_FLOAT_PRI$1, sizeof(_yIO_FLOAT_PRI$1) - 1);
}

static inline
int _yIO_float_astrfrom_asprintf$1(char **out, const char *fmt, _yIO_FLOAT$1 val) {
	const int err = asprintf(out, fmt, val);
	if (err <= 0) {
		return -ENOMEM;
	}
	return err;
}

static inline
int _yIO_float_astrfrom_snprintf$1(char **out, const char *fmt, _yIO_FLOAT$1 val) {
	const int len = snprintf(NULL, 0, fmt, val);
	assert(len > 0);
	char *buf = malloc(len + 1);
	if (buf == NULL) {
		*out = NULL;
		return -ENOMEM;
	}

	const int len2 = snprintf(buf, len + 1, fmt, val);
	(void)len2;
	assert(len2 == len);
	*out = buf;
	return len2;
}

int _yIO_float_astrfrom_printf$1(char **out, int precision, char spec, _yIO_FLOAT$1 val) {
	char fmt[FMT_SIZE$1];
	create_format_string$1(fmt, precision, spec);
#ifdef _yIO_HAS_asprintf
	return _yIO_float_astrfrom_asprintf$1(out, fmt, val);
#else
	return _yIO_float_astrfrom_snprintf$1(out, fmt, val);
#endif
}

#endif

~) m4_dnl;
