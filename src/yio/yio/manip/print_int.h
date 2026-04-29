/**
 * @file
 * @date 2024-04-27
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_INT_H_
#define YYIO_YIO_YIO_MANIP_PRINT_INT_H_

#include "../ctx_types.h"
#include <stdbool.h>

// No include ctx.h here, as it causes circular dependency.

#if YYIO_PRIVATE
// These functions are exported for printing bool, char and wchar_t as integers.
int YYIO_print_uchar_in(yio_printctx_t *t, unsigned char arg, bool is_negative);
int YYIO_print_ushort_in(yio_printctx_t *t, unsigned short arg, bool is_negative);
int YYIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YYIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);
int YYIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#if YYIO_HAS_INT128
int YYIO_print_u__int128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#endif
#endif

int YYIO_print_schar(yio_printctx_t *t);
int YYIO_print_uchar(yio_printctx_t *t);
int YYIO_print_short(yio_printctx_t *t);
int YYIO_print_ushort(yio_printctx_t *t);
int YYIO_print_int(yio_printctx_t *t);
int YYIO_print_uint(yio_printctx_t *t);
int YYIO_print_long(yio_printctx_t *t);
int YYIO_print_ulong(yio_printctx_t *t);
int YYIO_print_llong(yio_printctx_t *t);
int YYIO_print_ullong(yio_printctx_t *t);

#define YYIO_PRINT_INTS() \
	YYIO_OVERLOAD_TYPE_FUNC(short, YYIO_print_short) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned short, YYIO_print_ushort) \
	YYIO_OVERLOAD_TYPE_FUNC(int, YYIO_print_int) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned int, YYIO_print_uint) \
	YYIO_OVERLOAD_TYPE_FUNC(long, YYIO_print_long) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long, YYIO_print_ulong) \
	YYIO_OVERLOAD_TYPE_FUNC(long long, YYIO_print_llong) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long long, YYIO_print_ullong)

#ifndef YYIO_SCHAR_IS_UNIQUE
#error YYIO_SCHAR_IS_UNIQUE is not defiend
#endif
#if YYIO_SCHAR_IS_UNIQUE
#define YYIO_PRINT_SCHAR() \
        YYIO_OVERLOAD_TYPE_FUNC(signed char, YYIO_print_schar)
#else
#define YYIO_PRINT_SCHAR()
#endif

#ifndef YYIO_UCHAR_IS_UNIQUE
#error YYIO_UCHAR_IS_UNIQUE is not defiend
#endif
#if YYIO_UCHAR_IS_UNIQUE
#define YYIO_PRINT_UCHAR() \
        YYIO_OVERLOAD_TYPE_FUNC(unsigned char, YYIO_print_uchar)
#else
#define YYIO_PRINT_UCHAR()
#endif

#ifndef YYIO_HAS_INT128
#error YYIO_HAS_INT128 not defined
#endif
#if YYIO_HAS_INT128
int YYIO_print___int128(yio_printctx_t *t);
int YYIO_print_u__int128(yio_printctx_t *t);
#define YYIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		YYIO_OVERLOAD_TYPE_FUNC(__int128, YYIO_print___int128) \
		YYIO_OVERLOAD_TYPE_FUNC(unsigned __int128, YYIO_print_u__int128)
#else
#define YYIO_PRINT_FUNC_GENERIC_INTS_INT128()
#endif

/* ------------------------------------------------------------------------- */

#ifndef YYIO_BITINT_MAXWIDTH
#error YYIO_BITINT_MAXWIDTH not defined
#endif
#if YYIO_BITINT_MAXWIDTH

int YYIO_print_ubitint1(yio_printctx_t *t);
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %}
int YYIO_print_bitint{{i}}(yio_printctx_t *t);
int YYIO_print_ubitint{{i}}(yio_printctx_t *t);
{% endfor %}

#define YYIO_PRINT_FUNC_GENERIC_BITINTS() \
    YYIO_OVERLOAD_TYPE_FUNC(unsigned _BitInt(1), YYIO_print_ubitint1) \
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %}
    YYIO_OVERLOAD_TYPE_FUNC(_BitInt({{i}}), YYIO_print_bitint{{i}}) \
    YYIO_OVERLOAD_TYPE_FUNC(unsigned _BitInt({{i}}), YYIO_print_ubitint{{i}}) \
{% endfor %}

#else // YYIO_BITINT_MAXWIDTH
#define YYIO_PRINT_FUNC_GENERIC_BITINTS()
#endif // YYIO_BITINT_MAXWIDTH

#endif /* YYIO_YIO_YIO_MANIP_PRINT_INT_H_ */

