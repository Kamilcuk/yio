/**
 * @file yio_float_strfrom_strfrom.c
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define __STDC_WANT_IEC_60559_BFP_EXT__
m4_config();
#include "yio_float_strfrom_strfrom.h"
#include "yio_float_string.h"
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

m4_applyforeachdefine(`((f, f), (, d), (l, l))~, `m4_dnl;

extern int strfrom$2(char *str, size_t n, const char *format, _yIO_FLOAT$1 fp);

int _yIO_float_strfrom_strfrom$1(char *dest, int precision, char spec, _yIO_FLOAT$1 val) {
	char fmt[
			 sizeof("%") - 1 +
			 sizeof(".") - 1 +
			 INT_STRLEN_BOUND(int) +
			 sizeof(spec) +
			 1
	];

	char *fmtpnt = fmt;
	*fmtpnt++ = '%';
	if (precision >= 0) {
		*fmtpnt++ = '.';
		const size_t usedbytes = 2;
		const int len = snprintf(fmtpnt, sizeof(fmt) - usedbytes, "%d", precision);
		(void)len;
		assert(len > 0);
		assert((size_t)len < sizeof(fmt) - usedbytes);
		fmtpnt += len;
	}
	*fmtpnt++ = spec;
	*fmtpnt++ = '\0';
	assert(fmtpnt <= fmt + sizeof(fmt));

	return strfrom$2(dest, dest == NULL ? 0 : SIZE_MAX, fmt, val);
}

int _yIO_float_astrfrom_strfrom$1(char **out, int precision, char type, _yIO_FLOAT$1 val) {
	return _yIO_float_strfrom_to_astrfrom$1(out, precision, type, val, _yIO_float_strfrom_strfrom$1);
}

~) m4_dnl;
