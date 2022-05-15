/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_MANIP_MANIP_H_
#define _yIO_YIO_YΩIO_MANIP_MANIP_H_
#include "../ctx.h"
#include "../../yio_config.h"
#include "slots.h"
#include "unhandled.h"
#include "print_float.h"
#include "print_wchars.h"
#include "print_time.h"
#include "print_stdfix.h"
#include "print_complex.h"
#include "print_pfmt.h"
#include "print_mon.h"
#ifndef _yIO_HAS_UCHAR_H
#error _yIO_HAS_UCHAR_H
#endif
#if _yIO_HAS_UCHAR_H
#include <uchar.h>
#endif

int _yΩIO_print_bool(yπio_printctx_t *t);

int _yΩIO_print_char(yπio_printctx_t *t);
int _yΩIO_print_constcharpnt(yπio_printctx_t *t);

int _yΩIO_print_schar(yπio_printctx_t *t);
int _yΩIO_print_uschar(yπio_printctx_t *t);
int _yΩIO_print_short(yπio_printctx_t *t);
int _yΩIO_print_ushort(yπio_printctx_t *t);
int _yΩIO_print_int(yπio_printctx_t *t);
int _yΩIO_print_uint(yπio_printctx_t *t);
int _yΩIO_print_long(yπio_printctx_t *t);
int _yΩIO_print_ulong(yπio_printctx_t *t);
int _yΩIO_print_llong(yπio_printctx_t *t);
int _yΩIO_print_ullong(yπio_printctx_t *t);

#ifndef _yIO_HAS_INT128
#error
#endif
#if _yIO_HAS_INT128
int _yΩIO_print___int128(yπio_printctx_t *t);
int _yΩIO_print_u__int128(yπio_printctx_t *t);
#define _yΩIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		__int128: _yΩIO_print___int128, \
		unsigned __int128: _yΩIO_print_u__int128,
#else
#define _yΩIO_PRINT_FUNC_GENERIC_INTS_INT128()
#endif

#if _yIO_HAS_UCHAR_H
int _yΩIO_print_constchar16pnt(yπio_printctx_t *t);
int _yΩIO_print_constchar32pnt(yπio_printctx_t *t);
#define _yΩIO_PRINT_FUNC_GENERIC_UCHARS() \
		char16_t*: _yΩIO_print_constchar16pnt, \
		const char16_t*: _yΩIO_print_constchar16pnt, \
		char32_t*: _yΩIO_print_constchar32pnt, \
		const char32_t*: _yΩIO_print_constchar32pnt,
#else
#define _yΩIO_PRINT_FUNC_GENERIC_UCHARS()
#endif

int _yΩIO_print_voidp(yπio_printctx_t *t);

int _yΩIO_print_count(yπio_printctx_t *t);

/**
 * @define yπpcount(v)
 * @param v A pointer to an int.
 * This callback functions sets the pointed to integer by @c v to the
 * count of codepoints written by the function.
 */
#define yπpcount(v)  yiocb(_yΩIO_print_count, _Generic((v),int *:(v)))

#define _yΩIO_PRINT_FUNC_GENERIC_COUNT() \
		int *: _yΩIO_print_count,

/**
 * @def _yΩIO_PRINT_FUNC_GENERIC
 * For one argument choose the printing function dynamically using _Generic macro
 */
#define _yΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
			_yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
			bool: _yΩIO_print_bool, \
			char: _yΩIO_print_char, \
			char*: _yΩIO_print_constcharpnt, \
			const char*: _yΩIO_print_constcharpnt, \
			short: _yΩIO_print_short, \
			unsigned short: _yΩIO_print_ushort, \
			int: _yΩIO_print_int, \
			unsigned int: _yΩIO_print_uint, \
			long: _yΩIO_print_long, \
			unsigned long: _yΩIO_print_ulong, \
			long long: _yΩIO_print_llong, \
			unsigned long long: _yΩIO_print_ullong, \
			_yΩIO_PRINT_FUNC_GENERIC_INTS_INT128() \
			_yΩIO_PRINT_FUNC_GENERIC_WCHARS() \
			_yΩIO_PRINT_FUNC_GENERIC_UCHARS() \
			_yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
			_yΩIO_PRINT_GENERIC_TIME() \
			_yΩIO_PRINT_STDFIX() \
			_yΩIO_PRINT_COMPLEX() \
			void*: _yΩIO_print_voidp, \
			const void*: _yΩIO_print_voidp, \
		default: _Generic((arg), \
			signed char: _yΩIO_print_schar, \
			unsigned char: _yΩIO_print_uschar, \
			_yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		default: _yΩIO_print_unhandled_type \
		)))
#endif /* _yIO_YIO_YΩIO_MANIP_MANIP_H_ */
