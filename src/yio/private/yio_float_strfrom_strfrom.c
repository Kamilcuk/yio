/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#define __STDC_WANT_IEC_60559_BFP_EXT__  1
m4_config();
#include "yio_float_strfrom_strfrom.h"
#include <yio.h>
#include <yio/yio_config.h>
#include <yio/intprops.h>
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
		INT_STRLEN_BOUND(int) + \
		sizeof(char) + \
		1 \
)

static inline
void float_astrfrom_strfrom_create_format_string(char fmt[FMT_SIZE], int precision, char spec) {
	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision >= 0) {
		*fmtpnt++ = '.';
		const int len = ysprint(fmtpnt, INT_MAX, precision);
		(void)len;
		assert(len > 0);
		assert((size_t)len < FMT_SIZE - 2);
		fmtpnt += len;
	}
	*fmtpnt++ = spec;
	*fmtpnt++ = '\0';
	assert(fmtpnt <= fmt + FMT_SIZE);
}

m4_applyforeachdefine(`((f, f), (, d), (l, l))~, `m4_dnl;

// In case it's not defined in standard headers, so that we get a link time error.
extern int strfrom$2(char *str, size_t n, const char *format, _yIO_FLOAT$1 fp);

int _yIO_float_astrfrom_strfrom$1(char **out, int precision, char spec, _yIO_FLOAT$1 val) {
	// create format string
	char fmt[FMT_SIZE];
	float_astrfrom_strfrom_create_format_string(fmt, precision, spec);

	// get length
	int err = 0;
	const int len = strfrom$2(NULL, 0, fmt, val);
	if (len <= 0) {
		// this is not possible
		err = -ENOSYS;
		goto ERROR;
	}

	char * const buf = malloc(len + 1);
	if (buf == NULL) {
		err = -ENOMEM;
		goto ERROR;
	}

	const int len2 = strfrom$2(buf, len + 1, fmt, val);
	(void)len2;
	assert(len2 == len);

	*out = buf;
	return 0;
	ERROR:
	*out = NULL;
	return err;
}

~) m4_dnl;
