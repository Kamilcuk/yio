/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_TIME_H_
#define _yIO_YIO_PRIVATE_YIO_TIME_H_
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

#endif /* _yIO_YIO_PRIVATE_YIO_TIME_H_ */
