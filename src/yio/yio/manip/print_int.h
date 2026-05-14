/**
 * @file
 * @date 2024-04-27
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_INT_H_
#define YIO_YIO_YIO_MANIP_PRINT_INT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

int YIO_print_int(yio_printctx_t *t);
int YIO_print_uint(yio_printctx_t *t);
int YIO_print_long(yio_printctx_t *t);
int YIO_print_ulong(yio_printctx_t *t);

int YIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);

#if YIO_HAS_LLONG
int YIO_print_llong(yio_printctx_t *t);
int YIO_print_ullong(yio_printctx_t *t);
int YIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#define YIO_PRINT_LLONG(X, XALIAS) \
	X(long long, YIO_print_llong) \
	X(unsigned long long, YIO_print_ullong)
#else
#define YIO_PRINT_LLONG(X, XALIAS)
#endif

/* Signed types always promote to int */
#define YIO_print_schar YIO_print_int
#define YIO_print_short YIO_print_int

/* Unsigned char promotion alias */
#if UCHAR_MAX <= INT_MAX
#define YIO_print_uchar YIO_print_int
#else
#define YIO_print_uchar YIO_print_uint
#endif

/* Unsigned short promotion alias */
#if USHRT_MAX <= INT_MAX
#define YIO_print_ushort YIO_print_int
#else
#define YIO_print_ushort YIO_print_uint
#endif

#define YIO_PRINT_INTS(X, XALIAS) \
	XALIAS(short, YIO_print_short, short) \
	XALIAS(unsigned short, YIO_print_ushort, ushort) \
	X(int, YIO_print_int) \
	X(unsigned int, YIO_print_uint) \
	X(long, YIO_print_long) \
	X(unsigned long, YIO_print_ulong) \
	YIO_PRINT_LLONG(X, XALIAS)

#ifndef YIO_HAS_UNIQUE_SCHAR
#error YIO_HAS_UNIQUE_SCHAR is not defiend
#endif
#if YIO_HAS_UNIQUE_SCHAR
#define YIO_PRINT_SCHAR(X, XALIAS) \
        XALIAS(signed char, YIO_print_schar, schar)
#else
#define YIO_PRINT_SCHAR(X, XALIAS)
#endif

#ifndef YIO_HAS_UNIQUE_UCHAR
#error YIO_HAS_UNIQUE_UCHAR is not defiend
#endif
#if YIO_HAS_UNIQUE_UCHAR
#define YIO_PRINT_UCHAR(X, XALIAS) \
        XALIAS(unsigned char, YIO_print_uchar, uchar)
#else
#define YIO_PRINT_UCHAR(X, XALIAS)
#endif

#ifndef YIO_HAS_INT128
#error YIO_HAS_INT128 not defined
#endif
#if YIO_HAS_INT128
int YIO_print_int128(yio_printctx_t *t);
int YIO_print_uint128(yio_printctx_t *t);
int YIO_print_uint128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#define YIO_PRINT_FUNC_GENERIC_INTS_INT128(X, XALIAS) \
		X(__int128, YIO_print_int128) \
		X(unsigned __int128, YIO_print_uint128)
#else
#define YIO_PRINT_FUNC_GENERIC_INTS_INT128(X, XALIAS)
#endif

/* ------------------------------------------------------------------------- */

#ifndef YIO_INTERNAL_BITINT_MAXWIDTH
#error YIO_INTERNAL_BITINT_MAXWIDTH not defined
#endif
#if YIO_INTERNAL_BITINT_MAXWIDTH

int YIO_print_ubitint1(yio_printctx_t *t);
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %}
int YIO_print_bitint{{i}}(yio_printctx_t *t);
int YIO_print_ubitint{{i}}(yio_printctx_t *t);
{% endfor %}

#define YIO_PRINT_FUNC_GENERIC_BITINTS(X, XALIAS) \
    X(unsigned _BitInt(1), YIO_print_ubitint1) \
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %} \
    X(_BitInt({{i}}), YIO_print_bitint{{i}}) \
    X(unsigned _BitInt({{i}}), YIO_print_ubitint{{i}}) \
{% endfor %}

#else // YIO_INTERNAL_BITINT_MAXWIDTH
#define YIO_PRINT_FUNC_GENERIC_BITINTS(X, XALIAS)
#endif // YIO_INTERNAL_BITINT_MAXWIDTH


#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_INT_H_
