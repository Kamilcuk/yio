/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_MANIP_H_
#define YYIO_YIO_YΩIO_MANIP_MANIP_H_
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
#ifndef YIO_HAS_UCHAR_H
#error YIO_HAS_UCHAR_H
#endif
#if YIO_HAS_UCHAR_H
#include <uchar.h>
#endif

#if YYIO_PRIVATE
// These functions are exported for printing bool, char and wchar_t as integers.
int YYΩIO_print_uschar_in(yπio_printctx_t *t, unsigned char arg, bool is_negative);
int YYΩIO_print_ushort_in(yπio_printctx_t *t, unsigned short arg, bool is_negative);
int YYΩIO_print_uint_in(yπio_printctx_t *t, unsigned int arg, bool is_negative);
int YYΩIO_print_ulong_in(yπio_printctx_t *t, unsigned long arg, bool is_negative);
int YYΩIO_print_ullong_in(yπio_printctx_t *t, unsigned long long arg, bool is_negative);
#if YYIO_HAS_INT128
int YYΩIO_print_u__int128_in(yπio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#endif
#endif

int YYΩIO_print_bool(yπio_printctx_t *t);

int YYΩIO_print_char(yπio_printctx_t *t);
int YYΩIO_print_constcharpnt(yπio_printctx_t *t);

int YYΩIO_print_schar(yπio_printctx_t *t);
int YYΩIO_print_uschar(yπio_printctx_t *t);
int YYΩIO_print_short(yπio_printctx_t *t);
int YYΩIO_print_ushort(yπio_printctx_t *t);
int YYΩIO_print_int(yπio_printctx_t *t);
int YYΩIO_print_uint(yπio_printctx_t *t);
int YYΩIO_print_long(yπio_printctx_t *t);
int YYΩIO_print_ulong(yπio_printctx_t *t);
int YYΩIO_print_llong(yπio_printctx_t *t);
int YYΩIO_print_ullong(yπio_printctx_t *t);

#ifndef YYIO_HAS_INT128
#error
#endif
#if YYIO_HAS_INT128
int YYΩIO_print___int128(yπio_printctx_t *t);
int YYΩIO_print_u__int128(yπio_printctx_t *t);
#define YYΩIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		__int128: YYΩIO_print___int128, \
		unsigned __int128: YYΩIO_print_u__int128,
#else
#define YYΩIO_PRINT_FUNC_GENERIC_INTS_INT128()
#endif

#if YIO_HAS_UCHAR_H
int YYΩIO_print_constchar16pnt(yπio_printctx_t *t);
int YYΩIO_print_constchar32pnt(yπio_printctx_t *t);
#define YYΩIO_PRINT_FUNC_GENERIC_UCHARS() \
		char16_t*: YYΩIO_print_constchar16pnt, \
		const char16_t*: YYΩIO_print_constchar16pnt, \
		char32_t*: YYΩIO_print_constchar32pnt, \
		const char32_t*: YYΩIO_print_constchar32pnt,
#else
#define YYΩIO_PRINT_FUNC_GENERIC_UCHARS()
#endif

int YYΩIO_print_voidp(yπio_printctx_t *t);

int YYΩIO_print_count(yπio_printctx_t *t);

/**
 * @define yπpcount(v)
 * @param v A pointer to an int.
 * This callback functions sets the pointed to integer by @c v to the
 * count of codepoints written by the function.
 */
#define yπpcount(v)  yiocb(YYΩIO_print_count, _Generic((v),int *:(v)))

#define YYΩIO_PRINT_FUNC_GENERIC_COUNT() \
		int *: YYΩIO_print_count,

/**
 * @def YYΩIO_PRINT_FUNC_GENERIC
 * For one argument choose the printing function dynamically using _Generic macro
 */
#define YYΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
			YYΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
			bool: YYΩIO_print_bool, \
			char: YYΩIO_print_char, \
			char*: YYΩIO_print_constcharpnt, \
			const char*: YYΩIO_print_constcharpnt, \
			short: YYΩIO_print_short, \
			unsigned short: YYΩIO_print_ushort, \
			int: YYΩIO_print_int, \
			unsigned int: YYΩIO_print_uint, \
			long: YYΩIO_print_long, \
			unsigned long: YYΩIO_print_ulong, \
			long long: YYΩIO_print_llong, \
			unsigned long long: YYΩIO_print_ullong, \
			YYΩIO_PRINT_FUNC_GENERIC_INTS_INT128() \
			YYΩIO_PRINT_FUNC_GENERIC_WCHARS() \
			YYΩIO_PRINT_FUNC_GENERIC_UCHARS() \
			YYΩIO_PRINT_FUNC_GENERIC_FLOATS() \
			YYΩIO_PRINT_GENERIC_TIME() \
			YYΩIO_PRINT_STDFIX() \
			YYΩIO_PRINT_COMPLEX() \
			void*: YYΩIO_print_voidp, \
			const void*: YYΩIO_print_voidp, \
		default: _Generic((arg), \
			signed char: YYΩIO_print_schar, \
			unsigned char: YYΩIO_print_uschar, \
			YYΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		default: YYΩIO_print_unhandled_type \
		)))
#endif /* YYIO_YIO_YΩIO_MANIP_MANIP_H_ */
