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
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

int YYIO_print_int(yio_printctx_t *t);
int YYIO_print_uint(yio_printctx_t *t);
int YYIO_print_long(yio_printctx_t *t);
int YYIO_print_ulong(yio_printctx_t *t);

int YYIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YYIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);

#if YYIO_HAS_LLONG
int YYIO_print_llong(yio_printctx_t *t);
int YYIO_print_ullong(yio_printctx_t *t);
int YYIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#define YYIO_PRINT_LLONG(X, XALIAS) \
	X(long long, YYIO_print_llong) \
	X(unsigned long long, YYIO_print_ullong)
#else
#define YYIO_PRINT_LLONG(X, XALIAS)
#endif

/* Signed types always promote to int */
#define YYIO_print_schar YYIO_print_int
#define YYIO_print_short YYIO_print_int

/* Unsigned char promotion alias */
#if UCHAR_MAX <= INT_MAX
#define YYIO_print_uchar YYIO_print_int
#else
#define YYIO_print_uchar YYIO_print_uint
#endif

/* Unsigned short promotion alias */
#if USHRT_MAX <= INT_MAX
#define YYIO_print_ushort YYIO_print_int
#else
#define YYIO_print_ushort YYIO_print_uint
#endif

#define YYIO_PRINT_INTS(X, XALIAS) \
	XALIAS(short, YYIO_print_short, short) \
	XALIAS(unsigned short, YYIO_print_ushort, ushort) \
	X(int, YYIO_print_int) \
	X(unsigned int, YYIO_print_uint) \
	X(long, YYIO_print_long) \
	X(unsigned long, YYIO_print_ulong) \
	YYIO_PRINT_LLONG(X, XALIAS)

#ifndef YYIO_HAS_UNIQUE_SCHAR
#error YYIO_HAS_UNIQUE_SCHAR is not defiend
#endif
#if YYIO_HAS_UNIQUE_SCHAR
#define YYIO_PRINT_SCHAR(X, XALIAS) \
        XALIAS(signed char, YYIO_print_schar, schar)
#else
#define YYIO_PRINT_SCHAR(X, XALIAS)
#endif

#ifndef YYIO_HAS_UNIQUE_UCHAR
#error YYIO_HAS_UNIQUE_UCHAR is not defiend
#endif
#if YYIO_HAS_UNIQUE_UCHAR
#define YYIO_PRINT_UCHAR(X, XALIAS) \
        XALIAS(unsigned char, YYIO_print_uchar, uchar)
#else
#define YYIO_PRINT_UCHAR(X, XALIAS)
#endif

#ifndef YYIO_HAS_INT128
#error YYIO_HAS_INT128 not defined
#endif
#if YYIO_HAS_INT128
int YYIO_print_int128(yio_printctx_t *t);
int YYIO_print_uint128(yio_printctx_t *t);
int YYIO_print_uint128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#define YYIO_PRINT_FUNC_GENERIC_INTS_INT128(X, XALIAS) \
		X(__int128, YYIO_print_int128) \
		X(unsigned __int128, YYIO_print_uint128)
#else
#define YYIO_PRINT_FUNC_GENERIC_INTS_INT128(X, XALIAS)
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

#define YYIO_PRINT_FUNC_GENERIC_BITINTS(X, XALIAS) \
    X(unsigned _BitInt(1), YYIO_print_ubitint1) \
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %} \
    X(_BitInt({{i}}), YYIO_print_bitint{{i}}) \
    X(unsigned _BitInt({{i}}), YYIO_print_ubitint{{i}}) \
{% endfor %}

#else // YYIO_BITINT_MAXWIDTH
#define YYIO_PRINT_FUNC_GENERIC_BITINTS(X, XALIAS)
#endif // YYIO_BITINT_MAXWIDTH


#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_INT_H_
