/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

#if _yIO_HAS_UCHAR_H
#include <uchar.h>
int _yΩIO_print_char16pnt(yπio_printctx_t *t);
int _yΩIO_print_constchar16pnt(yπio_printctx_t *t);
int _yΩIO_print_char32pnt(yπio_printctx_t *t);
int _yΩIO_print_constchar32pnt(yπio_printctx_t *t);
#define _yΩIO_PRINT_FUNC_GENERIC_UCHARS() \
		char16_t*: _yΩIO_print_char16pnt, \
		const char16_t*: _yΩIO_print_constchar16pnt, \
		char32_t*: _yΩIO_print_char32pnt, \
		const char32_t*: _yΩIO_print_constchar32pnt,
#else
#define _yΩIO_PRINT_FUNC_GENERIC_UCHARS()
#endif

