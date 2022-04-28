/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#define _yIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#include "../yio_config.h"
#include "yio_res.h"
#include <stddef.h>
#include <time.h>

/**
 * Dynamic strftime.
 * The call @c strftime(..., fmt, tm) *must* result in a nonzero string.
 * @param res The @c _yIO_res object for storing output.
 * @param fmt The formatting string for strftime.
 *            It *must* result in a nonzero output from strftime.
 * @param tm The current broken down time representation.
 * @return 0 on success, otherwise error.
 */
int _yIO_astrftime_nonzero(_yIO_res *res, const char *fmt, const struct tm *tm);

#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if _yIO_HAS_MONETARY_H

struct _yIO_astrfmon_arg {
	union {
		double d;
		long double ld;
	} v;
	bool isldbl;
};

/**
 * Dynamic allocation astrfmon.
 * @param res The @c _yIO_res object for storing output.
 * @param fmt The formatting string passed to strfmon.
 * @param arg The arguments, which should match type with formatting string.
 * @return 0 on success, otherwise error.
 */
int _yIO_astrfmon(_yIO_res *res, const char *fmt, struct _yIO_astrfmon_arg arg);
#endif

#endif /* _yIO_YIO_PRIVATE_YIO_ALLOCHELP_H_ */
