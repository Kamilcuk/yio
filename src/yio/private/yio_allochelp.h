/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#define YYIO_YIO_PRIVATE_YIO_ALLOCHELP_H_
#include "../yio_config.h"
#include "yio_res.h"
#include <stddef.h>
#include <time.h>

/**
 * Dynamic strftime.
 * The call @c strftime(..., fmt, tm) *must* result in a nonzero string.
 * @param res The @c YYIO_res object for storing output.
 * @param fmt The formatting string for strftime.
 *            It *must* result in a nonzero output from strftime.
 * @param tm The current broken down time representation.
 * @return 0 on success, otherwise error.
 */
int YYIO_astrftime_nonzero(YYIO_res *res, const char *fmt, const struct tm *tm);

#ifndef YYIO_HAS_MONETARY_H
#error
#endif
#if YYIO_HAS_MONETARY_H

struct YYIO_astrfmon_arg {
	union {
		double d;
		long double ld;
	} v;
	bool isldbl;
};

/**
 * Dynamic allocation astrfmon.
 * @param res The @c YYIO_res object for storing output.
 * @param fmt The formatting string passed to strfmon.
 * @param arg The arguments, which should match type with formatting string.
 * @return 0 on success, otherwise error.
 */
int YYIO_astrfmon(YYIO_res *res, const char *fmt, struct YYIO_astrfmon_arg arg);
#endif

#endif /* YYIO_YIO_PRIVATE_YIO_ALLOCHELP_H_ */
