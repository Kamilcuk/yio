/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_MANIP_H_
#define YYIO_YIO_YIO_MANIP_MANIP_H_

/**
 * @def YYIO_COUNTER
 * Internal preprocessor counter for adding types into _Generic.
 * Incremented by including YIO_ADD_TYPE_INC file.
 */
#define YYIO_COUNTER  0

/**
 * @def YYIO_PRINT_FUNC_GENERIC_SLOTS
 * Callback for handling user _Generic types
 */
#define YYIO_PRINT_FUNC_GENERIC_SLOTS() /**/

#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx.h"
#include "../../yio_config.h"
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
int YYIO_print_uschar_in(yio_printctx_t *t, unsigned char arg, bool is_negative);
int YYIO_print_ushort_in(yio_printctx_t *t, unsigned short arg, bool is_negative);
int YYIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YYIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);
int YYIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#if YYIO_HAS_INT128
int YYIO_print_u__int128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#endif
#endif

int YYIO_print_bool(yio_printctx_t *t);

int YYIO_print_char(yio_printctx_t *t);
int YYIO_print_constcharpnt(yio_printctx_t *t);

int YYIO_print_schar(yio_printctx_t *t);
int YYIO_print_uschar(yio_printctx_t *t);
int YYIO_print_short(yio_printctx_t *t);
int YYIO_print_ushort(yio_printctx_t *t);
int YYIO_print_int(yio_printctx_t *t);
int YYIO_print_uint(yio_printctx_t *t);
int YYIO_print_long(yio_printctx_t *t);
int YYIO_print_ulong(yio_printctx_t *t);
int YYIO_print_llong(yio_printctx_t *t);
int YYIO_print_ullong(yio_printctx_t *t);

#ifndef YYIO_HAS_INT128
#error
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

#if YIO_HAS_UCHAR_H
int YYIO_print_constchar16pnt(yio_printctx_t *t);
int YYIO_print_constchar32pnt(yio_printctx_t *t);
#define YYIO_PRINT_FUNC_GENERIC_UCHARS() \
		YYIO_OVERLOAD_TYPE_FUNC(char16_t*, YYIO_print_constchar16pnt) \
		YYIO_OVERLOAD_TYPE_FUNC(const char16_t*, YYIO_print_constchar16pnt) \
		YYIO_OVERLOAD_TYPE_FUNC(char32_t*, YYIO_print_constchar32pnt) \
		YYIO_OVERLOAD_TYPE_FUNC(const char32_t*, YYIO_print_constchar32pnt)
#else
#define YYIO_PRINT_FUNC_GENERIC_UCHARS()
#endif

int YYIO_print_voidp(yio_printctx_t *t);

int YYIO_print_count(yio_printctx_t *t);

/**
 * @define yio_count(v)
 * @param v A pointer to an int.
 * This callback functions sets the pointed to integer by @c v to the
 * count of codepoints written by the function.
 */
#ifdef __cplusplus
#define yio_count(v)  yio_callback(YYIO_print_count, (v))
#else
#define yio_count(v)  yio_callback(YYIO_print_count, _Generic((v),int *:(v)))
#endif


#define YYIO_PRINT_FUNC_GENERIC_COUNT() \
		YYIO_OVERLOAD_TYPE_FUNC(int *, YYIO_print_count)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define YYIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC) \
	inline yio_printdata_t yyio_print_func_generic_cpp(TYPE) { return (yio_printdata_t)(FUNC); }
#else
#define YYIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC) \
	TYPE: FUNC,
#endif

#ifdef __cplusplus
namespace yyio_cpp {
	template <typename T> inline yio_printdata_t yyio_print_func_generic_cpp(const T&) { return (yio_printdata_t)YYIO_print_unhandled_type; }

	YYIO_OVERLOAD_TYPE_FUNC(bool, YYIO_print_bool)
	YYIO_OVERLOAD_TYPE_FUNC(char, YYIO_print_char)
	YYIO_OVERLOAD_TYPE_FUNC(char*, YYIO_print_constcharpnt)
	YYIO_OVERLOAD_TYPE_FUNC(const char*, YYIO_print_constcharpnt)
	YYIO_OVERLOAD_TYPE_FUNC(short, YYIO_print_short)
	YYIO_OVERLOAD_TYPE_FUNC(unsigned short, YYIO_print_ushort)
	YYIO_OVERLOAD_TYPE_FUNC(int, YYIO_print_int)
	YYIO_OVERLOAD_TYPE_FUNC(unsigned int, YYIO_print_uint)
	YYIO_OVERLOAD_TYPE_FUNC(long, YYIO_print_long)
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long, YYIO_print_ulong)
	YYIO_OVERLOAD_TYPE_FUNC(long long, YYIO_print_llong)
	YYIO_OVERLOAD_TYPE_FUNC(unsigned long long, YYIO_print_ullong)
	YYIO_PRINT_FUNC_GENERIC_INTS_INT128()
	YYIO_PRINT_FUNC_GENERIC_WCHARS()
	YYIO_PRINT_FUNC_GENERIC_UCHARS()
	YYIO_PRINT_FUNC_GENERIC_FLOATS()
	YYIO_PRINT_GENERIC_TIME()
	YYIO_PRINT_STDFIX()
	YYIO_PRINT_COMPLEX()
	YYIO_OVERLOAD_TYPE_FUNC(void*, YYIO_print_voidp)
	YYIO_OVERLOAD_TYPE_FUNC(const void*, YYIO_print_voidp)
	YYIO_OVERLOAD_TYPE_FUNC(signed char, YYIO_print_schar)
	YYIO_OVERLOAD_TYPE_FUNC(unsigned char, YYIO_print_uschar)
	YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()

	YYIO_PRINT_FUNC_GENERIC_SLOTS()
}
#endif

/**
 * @def YYIO_PRINT_FUNC_GENERIC
 * For one argument choose the printing function dynamically using _Generic macro
 */
#ifdef __cplusplus
#define YYIO_PRINT_FUNC_GENERIC(arg, ...) \
		yyio_cpp::yyio_print_func_generic_cpp(arg)
#else
#define YYIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
			YYIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
			bool: YYIO_print_bool, \
			char: YYIO_print_char, \
			char*: YYIO_print_constcharpnt, \
			const char*: YYIO_print_constcharpnt, \
			short: YYIO_print_short, \
			unsigned short: YYIO_print_ushort, \
			int: YYIO_print_int, \
			unsigned int: YYIO_print_uint, \
			long: YYIO_print_long, \
			unsigned long: YYIO_print_ulong, \
			long long: YYIO_print_llong, \
			unsigned long long: YYIO_print_ullong, \
			YYIO_PRINT_FUNC_GENERIC_INTS_INT128() \
			YYIO_PRINT_FUNC_GENERIC_WCHARS() \
			YYIO_PRINT_FUNC_GENERIC_UCHARS() \
			YYIO_PRINT_FUNC_GENERIC_FLOATS() \
			YYIO_PRINT_GENERIC_TIME() \
			YYIO_PRINT_STDFIX() \
			YYIO_PRINT_COMPLEX() \
			void*: YYIO_print_voidp, \
			const void*: YYIO_print_voidp, \
		default: _Generic((arg), \
			signed char: YYIO_print_schar, \
			unsigned char: YYIO_print_uschar, \
			YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
		default: YYIO_print_unhandled_type \
		)))
#endif

/**
 * @def YIO_ADD_TYPE_INC
 * Include this file after using YIO_ADD_TYPE macro to add addional type to handle.
 * Example:
 *
 *     struct my_type { int a; };
 *     int my_print_function(yio_printctx_t *ctx) { .... }
 *     // Print the type using this function. Note - no ; on the end.
 *     YIO_ADD_TYPE(struct my_type, my_print_function)
 *     // Include the file afterwards.
 *     #include YIO_ADD_TYPE_INC()
 *
 *     int main() {
 *        struct my_type var;
 *        yio_printf("{}", var);  // Will use my_print_function to print the variable.
 *     }
 */
#define YIO_ADD_TYPE_INC()   <yio/yio/manip/slots.h>  // NOLINT

/**
 * @def YIO_ADD_TYPE
 * @see YIO_ADD_TYPE_INC
 */
#ifdef __cplusplus
#define YIO_ADD_TYPE(TYPE, FUNCTION)   \
	typedef TYPE YYIO_XCONCAT(YYIO_TYPE_, YYIO_COUNTER); \
	static inline int YYIO_XCONCAT(YYIO_TYPE_FUNC_, YYIO_COUNTER)(yio_printctx_t *ctx) { return (FUNCTION)(ctx); } \
	namespace yyio_cpp { YYIO_OVERLOAD_TYPE_FUNC(TYPE, FUNCTION) }
#elif defined(__GNUC__)
#define YIO_ADD_TYPE(TYPE, FUNCTION)   \
	typedef typeof(TYPE) YYIO_XCONCAT(YYIO_TYPE_, YYIO_COUNTER); \
	static inline int YYIO_XCONCAT(YYIO_TYPE_FUNC_, YYIO_COUNTER)(yio_printctx_t *ctx) __attribute__((__alias__(#FUNCTION)));
#else
#define YIO_ADD_TYPE(TYPE, FUNCTION)   \
	typedef TYPE YYIO_XCONCAT(YYIO_TYPE_, YYIO_COUNTER); \
	static inline int YYIO_XCONCAT(YYIO_TYPE_FUNC_, YYIO_COUNTER)(yio_printctx_t *ctx) { return (FUNCTION)(ctx); }
#endif

#endif /* YYIO_YIO_YIO_MANIP_MANIP_H_ */
