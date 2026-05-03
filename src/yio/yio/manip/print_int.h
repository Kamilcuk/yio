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
#define YYIO_PRINT_LLONG() \
	YYIO_OVERLOAD_TYPE_FUNC(long long, YYIO_print_llong) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long long, YYIO_print_ullong)
#else
#define YYIO_PRINT_LLONG()
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

#define YYIO_PRINT_INTS() \
	YYIO_OVERLOAD_TYPE_FUNC(short, YYIO_print_short) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned short, YYIO_print_ushort) \
	YYIO_OVERLOAD_TYPE_FUNC(int, YYIO_print_int) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned int, YYIO_print_uint) \
	YYIO_OVERLOAD_TYPE_FUNC(long, YYIO_print_long) \
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long, YYIO_print_ulong) \
	YYIO_PRINT_LLONG()

#ifndef YYIO_HAS_UNIQUE_SCHAR
#error YYIO_HAS_UNIQUE_SCHAR is not defiend
#endif
#if YYIO_HAS_UNIQUE_SCHAR
#define YYIO_PRINT_SCHAR() \
        YYIO_OVERLOAD_TYPE_FUNC(signed char, YYIO_print_schar)
#else
#define YYIO_PRINT_SCHAR()
#endif

#ifndef YYIO_HAS_UNIQUE_UCHAR
#error YYIO_HAS_UNIQUE_UCHAR is not defiend
#endif
#if YYIO_HAS_UNIQUE_UCHAR
#define YYIO_PRINT_UCHAR() \
        YYIO_OVERLOAD_TYPE_FUNC(unsigned char, YYIO_print_uchar)
#else
#define YYIO_PRINT_UCHAR()
#endif

#ifndef YYIO_HAS_INT128
#error YYIO_HAS_INT128 not defined
#endif
#if YYIO_HAS_INT128
int YYIO_print_int128(yio_printctx_t *t);
int YYIO_print_uint128(yio_printctx_t *t);
int YYIO_print_uint128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#define YYIO_PRINT_FUNC_GENERIC_INTS_INT128() \
		YYIO_OVERLOAD_TYPE_FUNC(__int128, YYIO_print_int128) \
		YYIO_OVERLOAD_TYPE_FUNC(unsigned __int128, YYIO_print_uint128)
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

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_INT_H_
