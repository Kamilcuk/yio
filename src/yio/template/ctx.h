/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_CTX_H_
#define _yIO_YIO_YΩIO_CTX_H_
#include "../yio_common.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
typedef int _yΩIO_printcb_t(void *arg, const TCHAR *data, size_t count)
		_yIO_wur _yIO_nn(2) _yIO_access_r(2, 3);

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
int yπio_printctx_raw_write(yπio_printctx_t *t, const TCHAR *ptr, size_t size);

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

_yIO_wur _yIO_nn()
const TCHAR *yπio_printctx_get_fmtstrpnt(yπio_printctx_t *t);

/**
 * Internal callback called from @see yπio_printctx_print
 * @param t
 * @param data
 * @return
 */
_yIO_wur _yIO_nn(1, 2)
int _yΩIO_printctx_print(yπio_printctx_t *t, yπio_printdata_t *data, const TCHAR *fmt, ...);

_yIO_wur _yIO_nn()
int _yΩIO_printformat_generic(yπio_printctx_t *t,
		const TCHAR str[], size_t str_len, bool is_number, bool is_positive);

_yIO_wur _yIO_nn() static inline
int yπio_printctx_putπ(yπio_printctx_t *t, const TCHAR str[], size_t str_len) {
	return _yΩIO_printformat_generic(t, str, str_len, false, false);
}

{% if MODEX != 1 %}
_yIO_wur _yIO_nn()
int _yΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive);

_yIO_wur _yIO_nn() static inline
int yπio_printctx_put(yπio_printctx_t *t, const char str[], size_t str_len) {
	return _yΩIO_printformat_generic_char(t, str, str_len, false, false);
}

_yIO_wur _yIO_nn() static inline
int yπio_printctx_put_number(yπio_printctx_t *t, const char str[], size_t str_len, bool is_positive) {
	return _yΩIO_printformat_generic_char(t, str, str_len, true, is_positive);
}
{% endif %}

_yIO_wur _yIO_nn() static inline
int yπio_printctx_putπ_number(yπio_printctx_t *t, const TCHAR str[], size_t str_len, bool is_positive) {
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
_yIO_wur _yIO_nn() _yIO_access_r(1)
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
 * Get va_list from printctx.
 */
_yIO_wur _yIO_rnn _yIO_nn() _yIO_access_r(1)
va_list *_yΩIO_printctx_get_va(yπio_printctx_t *t);

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
#define yπio_printctx_va_arg(printctx, type)   va_arg(*_yΩIO_printctx_get_va(printctx), type)

/**
 * Automatically promote the type for integer types.
 * Argument has to be an arithmetic type, so that it can be promoted.
 * @def yπio_printctx_va_arg_num(printctx, numtype)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param numtype Numericall type, that arithmetics can be done for.
 * @return A value from the printcts va_list of type promoted numtype.
 */
#define yπio_printctx_va_arg_num(printctx, numtype)  \
		_Generic(+(numtype)1, \
			int: yπio_printctx_va_arg(printctx, int), \
			unsigned: yπio_printctx_va_arg(printctx, unsigned), \
			float: yπio_printctx_va_arg(printctx, double), \
			default: yπio_printctx_va_arg(printctx, numtype)  /* NOLINT(clang-diagnostic-varargs) */ \
		)

/**
 * @}
 */

#endif /* _yIO_YIO_YΩIO_CTX_H_ */
