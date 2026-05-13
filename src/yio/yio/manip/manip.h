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
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx.h"
#include "../../yio_config.h"
#include "print_float.h"
#include "print_wchars.h"
#include "print_tm.h"
#include "print_timevalspec.h"
#include "print_stdfix.h"
#include "print_complex.h"
#include "print_pfmt.h"
#include "print_mon.h"
#include "print_int.h"
#include "print_repr.h"

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


#ifndef YYIO_HAS_UNIQUE_CONSTPOINTER
#error YYIO_HAS_UNIQUE_CONSTPOINTER is not defined
#endif

#ifdef __cplusplus
#define YYIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC) \
	inline yio_printdata_t yyio_print_func_generic_cpp(TYPE) { return (yio_printdata_t)(FUNC); }
#define YYIO_OVERLOAD_POINTER_TYPE_FUNC(BASETYPE, FUNC) \
	YYIO_OVERLOAD_TYPE_FUNC(BASETYPE *, FUNC) \
	YYIO_OVERLOAD_TYPE_FUNC(const BASETYPE *, FUNC)
#else // __cplusplus
#define YYIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC)  TYPE: FUNC,
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
#define YYIO_OVERLOAD_POINTER_TYPE_FUNC(BASETYPE, FUNC) \
		TYPE __xdata *: FUNC, \
		TYPE __code *: FUNC, \
		TYPE __pdata *: FUNC, \
		TYPE __idata *: FUNC, \
		TYPE __data *: FUNC,
#elif defined(__SDCC_stm8)
#define YYIO_OVERLOAD_POINTER_TYPE_FUNC(BASETYPE, FUNC) BASETYPE *: FUNC,
#else // __SDCC
#define YYIO_OVERLOAD_POINTER_TYPE_FUNC(BASETYPE, FUNC) \
	BASETYPE *: FUNC, \
	YYIO_IF(YYIO_HAS_UNIQUE_CONSTPOINTER, const BASETYPE *: FUNC,)
#endif // __SDCC
#endif // __cplusplus

#ifndef YIO_HAS_UCHAR_H
#error YIO_HAS_UCHAR_H
#endif
#if YIO_HAS_UCHAR_H
#include <uchar.h>
int YYIO_print_constchar16pnt(yio_printctx_t *t);
int YYIO_print_constchar32pnt(yio_printctx_t *t);
#define YYIO_PRINT_FUNC_GENERIC_UCHARS() \
		YYIO_OVERLOAD_POINTER_TYPE_FUNC(char16_t, YYIO_print_constchar16pnt) \
		YYIO_OVERLOAD_POINTER_TYPE_FUNC(char32_t, YYIO_print_constchar32pnt)
#else
#define YYIO_PRINT_FUNC_GENERIC_UCHARS()
#endif

// print_bool.c
int YYIO_print_bool(yio_printctx_t *t);
// print_chars.c
int YYIO_print_char(yio_printctx_t *t);
int YYIO_print_constcharpnt(yio_printctx_t *t);
// print_voidp.c
int YYIO_print_voidp(yio_printctx_t *t);
// print_count.c
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
extern "C++" {
namespace yyio_cpp {
	YYIO_PRINT_FUNC_GENERIC_SLOTS()
	YYIO_PRINT_SCHAR()
	YYIO_PRINT_UCHAR()
	YYIO_PRINT_INTS()
	YYIO_PRINT_FUNC_GENERIC_INTS_INT128()
	YYIO_PRINT_FUNC_GENERIC_BITINTS()
	YYIO_PRINT_FUNC_GENERIC_WCHARS()
	YYIO_PRINT_FUNC_GENERIC_UCHARS()
	YYIO_PRINT_FUNC_GENERIC_FLOATS()
	YYIO_PRINT_GENERIC_TIMESPEC()
	YYIO_PRINT_GENERIC_TIMEVAL()
	YYIO_PRINT_GENERIC_TM()
	YYIO_PRINT_STDFIX()
	YYIO_PRINT_COMPLEX()
	YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE()
	YYIO_OVERLOAD_TYPE_FUNC(bool, YYIO_print_bool)
	YYIO_OVERLOAD_TYPE_FUNC(char, YYIO_print_char)
	YYIO_OVERLOAD_POINTER_TYPE_FUNC(char, YYIO_print_constcharpnt)
	YYIO_OVERLOAD_POINTER_TYPE_FUNC(void, YYIO_print_voidp)
}
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
#ifdef __SDCC
// In sdcc _Generic("string", char[7]:1) matches.
// To decay a string into a pointer, ternary expression can be used.
#define YYIO_DECAY(arg) 1?(arg):(arg)
#else
#define YYIO_DECAY(arg) (arg)
#endif
#define YYIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic(YYIO_DECAY(arg), \
			YYIO_PRINT_FUNC_GENERIC_SLOTS() \
			YYIO_PRINT_SCHAR() \
			YYIO_PRINT_UCHAR() \
			YYIO_PRINT_INTS() \
			YYIO_PRINT_FUNC_GENERIC_INTS_INT128() \
			YYIO_PRINT_FUNC_GENERIC_BITINTS() \
			YYIO_PRINT_FUNC_GENERIC_WCHARS() \
			YYIO_PRINT_FUNC_GENERIC_UCHARS() \
			YYIO_PRINT_FUNC_GENERIC_FLOATS() \
			YYIO_PRINT_GENERIC_TIMESPEC() \
			YYIO_PRINT_GENERIC_TIMEVAL() \
			YYIO_PRINT_GENERIC_TM() \
			YYIO_PRINT_STDFIX() \
			YYIO_PRINT_COMPLEX() \
			YYIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
			YYIO_OVERLOAD_POINTER_TYPE_FUNC(char, YYIO_print_constcharpnt) \
			YYIO_OVERLOAD_POINTER_TYPE_FUNC(void, YYIO_print_voidp) \
			bool: YYIO_print_bool, \
			char: YYIO_print_char \
		)
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

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_MANIP_H_
