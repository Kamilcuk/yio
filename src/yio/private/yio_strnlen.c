/**
 * @file
 * @date 2020-lip-30
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _POSIX_C_SOURCE  200809L
#define _GNU_SOURCE  1
#include "yio_strnlen.h"
#include "private.h"
#include <string.h>

size_t _yIO_strnlen(const char *s, size_t maxlen) {
#ifdef _yIO_HAS_strnlen
	return strnlen(s, maxlen);
#else
	const char *s0 = s;
	while (maxlen-- > 0 && *s) {
		++s;
	}
	return s - s0;
#endif
}

size_t _yIO_wcsnlen(const wchar_t *s, size_t maxlen) {
#ifdef _yIO_HAS_strnlen
	return wcsnlen(s, maxlen);
#else
	const wchar_t *s0 = s;
	while (maxlen-- > 0 && *s) {
			++s;
		}
		return s - s0;
	#endif
}
