/**
 * @file yio_float_strfrom_printf.c
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config()
#include "yio_float_strfrom_printf.h"
#include "yio_float_string.h"
#include <yio/intprops.h>
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

m4_applyforeachdefine(`((f), (), (l))~, `m4_dnl;

int _yIO_float_strfrom_printf$1(char *dest, int precision, char spec, _yIO_FLOAT$1 val) {
	char fmt[
			 sizeof("%") - 1 +
			 sizeof(".") - 1 +
			 INT_STRLEN_BOUND(int) +
			 sizeof(_yIO_FLOAT_PRI$1) - 1 +
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
	memcpy(fmtpnt, _yIO_FLOAT_PRI$1, sizeof(_yIO_FLOAT_PRI$1) - 1);
	fmtpnt += sizeof(_yIO_FLOAT_PRI$1) - 1;
	*fmtpnt++ = spec;
	*fmtpnt++ = '\0';
	assert(fmtpnt <= fmt + sizeof(fmt));

	return snprintf(dest, dest == NULL ? 0 : SIZE_MAX, fmt, val);
}

int _yIO_float_astrfrom_printf$1(char **out, int precision, char type, _yIO_FLOAT$1 val) {
	return _yIO_float_strfrom_to_astrfrom$1(out, precision, type, val, _yIO_float_strfrom_printf$1);
}

~) m4_dnl;
