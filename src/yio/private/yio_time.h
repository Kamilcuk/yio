/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

/**
 * Dynamic strftime.
 * The call `strftime(..., fmt, tm)` _must_ result in a nonzero string.
 * @param strp A pointer to buffer, just like for asprinf.
 * @param initialsize The initial allocation that is known to fail.
 * @param fmt The formatting string for strftime.
 * @param tm The current broken down time representation.
 * @return Number of bytes written or -1 on error
 */
int _yIO_astrftime_nonzero(char **strp, const size_t initialsize, const char *fmt, const struct tm *tm);

