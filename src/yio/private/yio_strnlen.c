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
#ifdef _yIO_HAS_UNISTRING
#include <unistr.h>
#endif

size_t _yIO_strnlen(const char *str, size_t maxlen) {
#ifdef _yIO_HAS_strnlen
	return strnlen(str, maxlen);
#else
	const char *str0 = str;
	while (maxlen-- > 0 && *str) {
		++s;
	}
	return str - str0;
#endif
}

size_t _yIO_wcsnlen(const wchar_t *str, size_t maxlen) {
#ifdef _yIO_HAS_strnlen
	return wcsnlen(str, maxlen);
#else
	const wchar_t *str0 = str;
	while (maxlen-- > 0 && *str) {
			++s;
		}
		return str - str0;
#endif
}

#ifdef _yIO_HAS_UCHAR_H

size_t _yIO_c16nlen(const char16_t *str, size_t maxlen) {
#ifdef _yIO_HAS_UNISTRING
	return u16_strnlen(str, maxlen);
#else
	const wchar16_t *str0 = str;
	while (maxlen-- && *str) {
		str++;
	}
	return str - str0;
#endif
}

size_t _yIO_c32nlen(const char32_t *str, size_t maxlen) {
#ifdef _yIO_HAS_UNISTRING
	return u32_strnlen(str, maxlen);
#else
	const wchar32_t *str0 = str;
	while (maxlen-- && *str) {
		str++;
	}
	return str - str0;
#endif
}

#endif
