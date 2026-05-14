/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#define YIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include "yio_string.h"
#include <stddef.h>
#include <time.h>

#ifndef YIO_HAS_FLOATl
#error
#endif

/**
 * Dynamic strftime.
 * The call @c strftime(..., fmt, tm) *must* result in a nonzero string.
 * @param res The @c YIO_string object for storing output.
 * @param fmt The formatting string for strftime.
 *            It *must* result in a nonzero output from strftime.
 * @param tm The current broken down time representation.
 * @return 0 on success, otherwise error.
 */
int YIO_astrftime_nonzero(YIO_string *res, const char *fmt, const struct tm *tm);

#ifndef YIO_HAS_MONETARY_H
#error
#endif
#if YIO_HAS_MONETARY_H

struct YIO_astrfmon_arg {
	union {
		double d;
#if YIO_HAS_FLOATl
		long double ld;
#endif
	} v;
	bool isldbl;
};

/**
 * Dynamic allocation astrfmon.
 * @param res The @c YIO_string object for storing output.
 * @param fmt The formatting string passed to strfmon.
 * @param arg The arguments, which should match type with formatting string.
 * @return 0 on success, otherwise error.
 */
int YIO_astrfmon(YIO_string *res, const char *fmt, struct YIO_astrfmon_arg arg);
#endif

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
