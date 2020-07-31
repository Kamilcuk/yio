/**
 * @file
 * @date 2020-lip-30
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "private.h"
#include <stddef.h>

_yIO_wur
size_t _yIO_strnlen(const char *s, size_t maxlen);

#ifdef _yIO_HAS_WCHAR_H
#include <wchar.h>
_yIO_wur
size_t _yIO_wcsnlen(const wchar_t *s, size_t maxlen);
#endif

#ifdef _yIO_HAS_UCHAR_H
#include <uchar.h>
_yIO_wur
size_t _yIO_c16nlen(const char16_t *str, size_t maxlen);
_yIO_wur
size_t _yIO_c32nlen(const char32_t *str, size_t maxlen);
#endif
