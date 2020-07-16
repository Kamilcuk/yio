/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include <stddef.h>
#include <time.h>

/**
 * Dynamic strftime.
 * The call @c strftime(..., fmt, tm) *must* result in a nonzero string.
 * @param strp A pointer to buffer, just like for asprinf.
 * @param initialsize The initial allocation that is known to fail.
 * @param fmt The formatting string for strftime.
 *            It *must* result in a nonzero output from strftime.
 * @param tm The current broken down time representation.
 * @return Number of bytes written or -1 on error
 */
int _yIO_astrftime_nonzero(char **strp, const size_t initialsize, const char *fmt, const struct tm *tm);

