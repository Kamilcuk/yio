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
#include <stddef.h>
#include <time.h>

/**
 * Dynamic strftime.
 * The call @c strftime(..., fmt, tm) *must* result in a nonzero string.
 * @param strp A pointer to allocated static buffer.
 * @param initialsize The initial allocation.
 * @param fmt The formatting string for strftime.
 *            It *must* result in a nonzero output from strftime.
 * @param tm The current broken down time representation.
 * @return Number of bytes written or -1 on error
 */
int _yIO_astrftime_nonzero(char **strp, const size_t initialsize, const char *fmt, const struct tm *tm);

#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if _yIO_HAS_MONETARY_H
/**
 * Dynamic allocation astrfmon.
 */
int _yIO_astrfmon(char **strp, size_t bufsize, const char *fmt, double dd);
#endif

#endif /* _yIO_YIO_PRIVATE_YIO_ALLOCHELP_H_ */
