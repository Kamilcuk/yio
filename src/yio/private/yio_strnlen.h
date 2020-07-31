/**
 * @file
 * @date 2020-lip-30
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include <stddef.h>

size_t _yIO_strnlen(const char *s, size_t maxlen);

size_t _yIO_wcsnlen(const wchar_t *s, size_t maxlen);
