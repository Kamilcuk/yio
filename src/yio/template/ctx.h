/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../yio_common.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

/* printctx ---------------------------------------------------------------------------- */

/**
 * @defgroup printctx Yio print context
 * @{
 */
/**
 * Print context.
 */
typedef struct _yΩIO_printctx_s yπio_printctx_t;

/**
 * Write data to output stream.
 * @param arg Custom argument passed with callback registration.
 * @param data The pointer to the data to print.
 * @param count Count of characters to print
 * @return 0 on success, anything else on error
 */
typedef int _yΩIO_printcb_t(void *arg, const Ychar *data, size_t count)
		_yIO_wur _yIO_nn(2) _yIO_access(__read_only__, 2, 3);

/**
 * The type of callback functions.
 * It is __expected__ that the first thing the function will do is read the arguments
 * from va_list. Even if function fails, the @c *va pointer __has to__ be correctly
 * updated with the arguments read.
 * This is the most important contract.
 * @param t Printing context.
 * @return 0 on success, anything else on error.
 */
typedef int (*_yΩIO_printfunc_t)(yπio_printctx_t *t);

/**
 * Get's passed to generic printing functions as first argument.
 * Array of printing function pointers pointers.
 */
typedef const _yΩIO_printfunc_t yπio_printdata_t;

_yIO_wur _yIO_nn()
int yπio_printctx_init(yπio_printctx_t *t);

/**
 * Write size count of bytes from ptr to output stream.
 * @param t
 * @param ptr
 * @param size
 * @return 0 on success, anything else on error.
 */
_yIO_wur _yIO_nn()
int yπio_printctx_raw_write(yπio_printctx_t *t, const Ychar *ptr, size_t size);

/**
 * Calls the next context parsing function in chain.
 * To be used by formatting-setting functions.
 * @param t
 * @return The return value of the next formatting function.
 */
_yIO_wur _yIO_nn()
int yπio_printctx_next(yπio_printctx_t *t);

/**
 * Get print context formatting options.
 * @param t
 * @return A valid pointer.
 */
_yIO_wur _yIO_nn() _yIO_const
struct yπio_printfmt_s *yπio_printctx_get_fmt(yπio_printctx_t *t);

/**
 * Internal callback called from @see yπio_printctx_print
 * @param t
 * @param data
 * @return
 */
_yIO_wur _yIO_nn(1, 2)
int _yΩIO_printctx_print(yπio_printctx_t *t, yπio_printdata_t *data, const Ychar *fmt, ...);

_yIO_wur _yIO_nn()
int _yΩIO_printformat_generic(yπio_printctx_t *t,
		const Ychar str[], size_t str_len, bool is_number, bool is_positive);

_yIO_wur _yIO_nn()
int _yΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive);

#if !_yIO_TYPE_YIO
_yIO_wur _yIO_nn() static inline
int yπio_printctx_put(yπio_printctx_t *t, const char str[], size_t str_len) {
	return _yΩIO_printformat_generic_char(t, str, str_len, false, false);
}
#endif

_yIO_wur _yIO_nn() static inline
int yπio_printctx_putπ(yπio_printctx_t *t, const Ychar str[], size_t str_len) {
	return _yΩIO_printformat_generic(t, str, str_len, false, false);
}

#if !_yIO_TYPE_YIO
_yIO_wur _yIO_nn() static inline
int yπio_printctx_put_number(yπio_printctx_t *t, const char str[], size_t str_len, bool is_positive) {
	return _yΩIO_printformat_generic_char(t, str, str_len, true, is_positive);
}
#endif

_yIO_wur _yIO_nn() static inline
int yπio_printctx_putπ_number(yπio_printctx_t *t, const Ychar str[], size_t str_len, bool is_positive) {
	return _yΩIO_printformat_generic(t, str, str_len, true, is_positive);
}

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... The printing parameters to print.
 * @return int 0 on success, anything else on error
 */
#define yπio_printctx_print(printctx, ...)   \
		_yΩIO_printctx_print(printctx, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))

/**
 * Internal callback called from @see yπio_printctx_va_arg*
 * @param t
 * @param sizeof_realtype
 * @return
 */
_yIO_wur _yIO_nn()
va_list *_yΩIO_printctx_inc_va_list(yπio_printctx_t *t, size_t sizeof_realtype);

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... the printing parameters to print.
 * @return int 0 on success, anything else on error
 */
#define yπio_printctx_printf(printctx, ...)  \
		_yΩIO_printctx_print(printctx, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Get next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type, see _yΩIO_IS_PROMOTED_TYPE
 * @def yπio_printctx_va_arg(printctx, type)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param type Type of argument passed to va_list.
 * @return A value from the printcts va_list of type type.
 */
#define yπio_printctx_va_arg(printctx, type)  yπio_printctx_va_arg2(printctx, type, type)

/**
 * Get next argument of type 'realtype' from arguments stack.
 * The real argument type after implicit conversion is 'promotedtype'.
 * This function has only usage for 'realtype' equal to char, short or float.
 * @def yπio_printctx_va_arg2(printctx, realtype, promotedtype)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param realtype The real type that was passed to va_list.
 * @param promotedtype The promoted type of real type.
 * @return A value from the printcts va_list of type promotedtype.
 */
#define yπio_printctx_va_arg2(printctx, realtype, promotedtype)  \
		va_arg(*_yΩIO_printctx_inc_va_list(printctx, sizeof(realtype)), promotedtype)

/**
 * Automatically promote the type for integer types.
 * Argument has to be an arithmetic type, so that it can be promoted.
 * @def yπio_printctx_va_arg_num(printctx, numtype)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param numtype Numericall type, that arithmetics can be done for.
 * @return A value from the printcts va_list of type promoted numtype.
 */
#ifndef __CDT_PARSER__
#define yπio_printctx_va_arg_num(printctx, numtype)  _Generic(+(numtype)1,\
		int: yπio_printctx_va_arg2(printctx, numtype, int), \
		unsigned: yπio_printctx_va_arg2(printctx, numtype, unsigned), \
		float: yπio_printctx_va_arg2(printctx, numtype, double), \
		default: yπio_printctx_va_arg2(printctx, numtype, numtype) \
)
#else
// CDT_PARSER doesn't support _Generic
#define yπio_printctx_va_arg_num(printctx, numtype)  \
		yπio_printctx_va_arg2(printctx, numtype, numtype)
#endif

/**
 * @}
 */

/* scanctx --------------------------------------------------------------------------- */

/**
 * @defgroup scanctx Yio scanning context
 * @{
 */
/**
 * Forward declaration of yπio_scanctx_t.
 * This is an opaque structure not visible to users.
 * Use yπio_scanctx_* functions to access it.
 */
typedef struct _yΩIO_scanctx_s yπio_scanctx_t;

/**
 * Scanner function.
 * @param t Pointer to scanning context.
 * @return 0 on success, anything else on error.
 */
typedef int (*_yΩIO_scanfunc_t)(yπio_scanctx_t *t);

/**
 * Read one byte from the input.
 * @param arg Custom user context.
 * @param data A valid character or YEOF.
 * @return 0 on success, anything else on error
 */
typedef int _yΩIO_scancb_t(void *arg, Yint *data)
	_yIO_wur _yIO_nn(2) _yIO_access(__write_only__, 2);

/**
 * A list of arguments created by macro substitution.
 */
typedef const struct _yΩIO_scandata_s {
	/**
	 * Array of scanning function pointers pointers.
	 */
	const _yΩIO_scanfunc_t * const funcs;
	/**
	 * Array of destination memory sizes.
	 */
	const size_t * const argpntsizes;
} yπio_scandata_t;

/**
 * Read one character from input and save it in c.
 * @param t
 * @param c
 * @return 0 on success, otherwise error.
 */
_yIO_wur _yIO_nn() _yIO_access(__read_only__, 2)
int yπio_scanctx_in(yπio_scanctx_t *t, Yint *c);

/**
 * Does the same as ungetc.
 * The next call to yπio_scanctx_in will re-return last character.
 * Should be called as the last function when scanning ended.
 * @param t
 */
_yIO_nn()
void yπio_scanctx_unin(yπio_scanctx_t *t);

/**
 * Call the next scanning function.
 * Should be called by formatting functions.
 * @param t
 * @return The return value of the next scanning function.
 */
_yIO_wur _yIO_nn()
int yπio_scanctx_next(yπio_scanctx_t *t);

/**
 * Get formatting options.
 * @param t
 * @return The pointer to scanning formatting options.
 */
_yIO_wur _yIO_nn() _yIO_const
struct yπio_scanfmt_s *yπio_scanctx_get_fmt(yπio_scanctx_t *t);

/**
 * Get the buffer destination size of the current argument.
 * When for example <tt>char pnt[20]; yscan(pnt);</tt>
 * Then this function will return <tt>20</tt>.
 * @param t
 * @return
 */
_yIO_wur _yIO_nn()
size_t yπio_scanctx_arg_size_next(yπio_scanctx_t *t);

/**
 * Internal callback called by @see yπio_scanctx_scan
 * @param t
 * @param data
 * @return
 */
_yIO_wur _yIO_nn(1, 2)
int _yΩIO_scanctx_scan(yπio_scanctx_t *t, yπio_scandata_t *data, const Ychar *fmt, ...);

/**
 * @def yπio_scanctx_scan(scanctx, ...)
 * @brief Scan input from the scanning context
 * @param scanctx Scanning context
 * @param ... Yio arguments list
 * @return 0 on success, otherwise error.
 */
#define yπio_scanctx_scan(scanctx, ...)   _yΩIO_scanctx_scan(scanctx, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))

/**
 * @def yπio_scanctx_scanf(scanctx, ...)
 * @brief Scan input from the scanning context
 * @param scanctx Scanning context
 * @param ... Yio format string
 * @param ... Yio arguments list
 * @return 0 on success, otherwise error.
 */
#define yπio_scanctx_scanf(scanctx, ...)  _yΩIO_scanctx_scan(scanctx, YIO_SCAN_ARGUMENTS(__VA_ARGS__))

/**
 * Increment to the next element in va_list.
 * @param t
 * @param sizeof_realtype The sizeof of the real type in va_list,
 *        for safety pseudo-type checking.
 * @return Pointer to va_list.
 */
_yIO_wur _yIO_nn()
va_list *_yΩIO_scanctx_inc_va_list(yπio_scanctx_t *t, size_t sizeof_realtype);

/**
 * @def yπio_scanctx_va_arg(scanctx, type)
 * @param scanctx Context to yπio_scanctx_t
 * @param type Type
 * @return A variable of passed type from va_list.
 * Get next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type,
 * see #_yΩIO_IS_PROMOTED_TYPE
 */
#define yπio_scanctx_va_arg(scanctx, type)  \
		va_arg(*_yΩIO_scanctx_inc_va_list(scanctx, sizeof(type)), type)

/**
 * @}
 */

