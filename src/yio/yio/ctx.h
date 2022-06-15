/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_CTX_H_
#define YYIO_YIO_YΩIO_CTX_H_
#include "../yio_config.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup yπio_printctx TMODEN yπio_printctx
 * @ingroup yπio
 * @brief Printing context.
 * @{
 */

/* types ------------------------------------------------------------------------- */

/// Printing formatting options.
struct yπio_printfmt_s {
	/// The field width. -1 when unset.
	int width;
	/// The field precision. -1 when unset.
	int precision;
	/// Filling character.
	TCHAR fill;
	/// May be one of '<' '>' '^' '=' characters or 0 when unset.
	TCHAR align;
	/// May be on of '+' '-' ' ' character or 0 when unset.
	TCHAR sign;
	/// May be set to 'L' or '_' ',' or 0 when unset.
	TCHAR grouping;
	/// The printing type character. The standard valid characters are "bcdeEfFgGnosxXp".
	TCHAR type;
	/// The conversion specifier. Do not mix with type!
	TCHAR c_onversion;
	/// Use of alternate form.
	bool hash;
};

/**
 * The default values of printfmt.
 */
extern const struct yπio_printfmt_s YYΩIO_printfmt_default;

/**
 * Print context.
 */
typedef struct YYΩIO_printctx_s yπio_printctx_t;

/**
 * The type representing a callback that will write output data to the user specified place.
 * @param arg Custom argument passed with callback registration.
 * @param data The pointer to the data to print.
 * @param count Count of characters to print
 * @return 0 on success, otherwise error
 */
typedef int YYΩIO_printcb_t(void *arg, const TCHAR *data, size_t count)
		YYIO_wur YYIO_nn(2) YYIO_access_r(2, 3);

/**
 * The type of callback functions, but abstractly, this represents
 * the "printing data" that are needed to represent the printing context.
 * Callback functions need to follow a contract, in order:
 * 1. Firstly, they need to eat the argument using @c yπio_printctx_va_arg.
 * 2. Optionally, then they need to parse the format string up until a '}'.
 * 3. Then, they need to call @c yπio_printctx_init.
 * 4. If _init returns non-zero, the value has to be returned.
 * 5. Then, there is a place for custom handling.
 * 6. The function should call @c yπio_printctx_put* functions and return with it's return value.
 * This contact is really important for making positional arguments work properly.
 * @param t Printing context.
 * @return 0 on success, otherwise error.
 */
typedef int (*const yπio_printdata_t)(yπio_printctx_t *t);

/**
 * The structure that allows for printing context manipulation.
 */
struct YYΩIO_printctx_s {
	/// Current iterator in the format string.
	const TCHAR *fmt;
	/// va_list of current argument.
	va_list *va;
	/// Copy of va_list when iterating
	va_list *startva;
	/// Iterator in callback functions.
	yπio_printdata_t *ifunc;
	/// The pointer to the data.
	yπio_printdata_t *startifunc;
	/// The outputting function.
	YYΩIO_printcb_t *out;
	/// User argument for outputting functions.
	void *outarg;
	/// The count of characters written.
	unsigned int writtencnt;
	/// Description of print formatting.
	struct yπio_printfmt_s pf;
	/// How many arguments to skip when outputting current argument.
	unsigned char skip;
};

/* functions ------------------------------------------------------------------------- */

/**
 * For positional arguments, arm the _skip_do function for @c count jumps.
 * @param t
 * @param count The positional number.
 */
void YYΩIO_skip_arm(yπio_printctx_t *t, unsigned int count);

/**
 * For positional arguments, skip until the proper positional argument is in va_arg.
 * @param t
 * @return 0 on success, otherwise error.
 */
int YYΩIO_skip_do(yπio_printctx_t *t);

/**
 * Convert the string pointed to by ptr to a digit.
 * The first character has to be a digit already, so it can't fail.
 * @param ptr
 * @return The converted number.
 */
int YYΩIO_printctx_strtoi_noerr(const TCHAR **ptr);

/**
 * Parse the width or precision param, that can be either a number of a positional parameter.
 */
int YYΩIO_printctx_stdintparam(yπio_printctx_t *t,
		const TCHAR *ptr, const TCHAR **endptr, int *res);

/**
 * Check if @c c is not nul and is one of characters in @c s.
 */
bool YYΩIO_strnulchrbool(const TCHAR *s, TCHAR c);

/**
 * Parse python formatting string.
 * @param c
 * @param pf
 * @param fmt Python formatting string like "{: < -0#123_.456}"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
YYIO_wur YYIO_nn()
int YYΩIO_pfmt_parse(yπio_printctx_t *c, struct yπio_printfmt_s *pf,
		const TCHAR *fmt, const TCHAR **endptr);

/* printctx ---------------------------------------------------- */

/**
 * This has to be the first function of a callback function.
 * Gets the next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type, see YYΩIO_IS_PROMOTED_TYPE
 * @def yπio_printctx_va_arg(printctx, type)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param type Type of argument passed to va_list.
 * @return A value from the printctx va_list of type type.
 * @see yπio_printctx_va_arg_promote
 */
#define yπio_printctx_va_arg(printctx, type)   va_arg(*(printctx)->va, type)

/**
 * Automatically promote the type for integer types.
 * Argument has to be an arithmetic type, so that it can be promoted.
 * @def yπio_printctx_va_arg_promote(printctx, numtype)
 * @param printctx Printing context, pointer to yπio_printctx_t
 * @param numtype Numericall type, that arithmetics can be done for.
 * @return A value from the printctx va_list of type promoted numtype.
 * @see yπio_printctx_va_arg
 */
#define yπio_printctx_va_arg_promote(printctx, numtype)  \
		_Generic(+(numtype)1, \
			int: yπio_printctx_va_arg(printctx, int), \
			unsigned int: yπio_printctx_va_arg(printctx, unsigned int), \
			float: yπio_printctx_va_arg(printctx, double), \
			default: yπio_printctx_va_arg(printctx, numtype)  /* NOLINT(clang-diagnostic-varargs) */ \
		)

/**
 * This function has to be called a callback right after calling va_arg.
 * This function checks if we are in skipping positional argument context,
 * if so, it will return with an error that should be propagated.
 * Otherwise, it will try to parse the format string assuming it to be
 * a python standard format string. If you want to parse the format string
 * yourself, leave the @c fmt pointer pointing at the closing '}'.
 * @param t
 * @return 0 on succes, otherwise error.
 */
YYIO_wur YYIO_nn()
int yπio_printctx_init(yπio_printctx_t *t);

/**
 * Write size count of bytes from ptr to output stream.
 * @param t
 * @param ptr
 * @param size
 * @return 0 on success, otherwise error.
 */
YYIO_wur YYIO_nn() YYIO_access_r(2, 3)
int yπio_printctx_raw_write(yπio_printctx_t *t, const TCHAR *ptr, size_t size);

/**
 * Calls the next context parsing function in chain.
 * To be used by formatting-setting functions.
 * @param t
 * @return The return value of the next formatting function.
 */
YYIO_wur YYIO_nn()
int yπio_printctx_next(yπio_printctx_t *t);

/**
 * Get print context formatting options.
 * @param t
 * @return A valid pointer.
 */
static inline
struct yπio_printfmt_s *yπio_printctx_get_fmt(yπio_printctx_t *t) {
	return &t->pf;
}

/**
 * Internal callback called from
 * @see yπio_printctx_print
 * @param t
 * @param data
 * @return
 */
YYIO_wur YYIO_nn(1, 2)
int YYΩIO_printctx_print_in(yπio_printctx_t *t, yπio_printdata_t *data, const TCHAR *fmt, ...);

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... the printing parameters to print.
 * @return int 0 on success, otherwise error
 */
#define yπio_printctx_printf(printctx, ...)  \
		YYΩIO_printctx_print_in(printctx, YΩIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Generic formatter to output stuff formatted according to python standard format specification.
 * @param t
 * @param str The string to output.
 * @param str_len Length of @c str.
 * @param is_number Is @c str a string of a number, consisting of only digits?
 * @param is_positive If @c str is a number, is it a positive or negative number?
 * @return 0 on success, otherwise error.
 */
YYIO_wur YYIO_nn()
int YYΩIO_printformat_generic(yπio_printctx_t *t,
		const TCHAR str[], size_t str_len, bool is_number, bool is_positive);

/**
 * From printing context output a string
 * @see YYΩIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yπio_printctx_putπ(yπio_printctx_t *t, const TCHAR str[], size_t str_len) {
	return YYΩIO_printformat_generic(t, str, str_len, false, false);
}

/**
 * From printing context output a number.
 * The @c str argument has to consist of only digits.
 * @see YYΩIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yπio_printctx_putπ_number(yπio_printctx_t *t, const TCHAR str[], size_t str_len, bool is_positive) {
	return YYΩIO_printformat_generic(t, str, str_len, true, is_positive);
}

#if TMODE != 1

/**
 * On modes different than normal, use this to output a @c char string.
 * Automatically converts @c char string into @c YCHAR string and then outputs it.
 */
YYIO_wur YYIO_nn()
int YYΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive);

/**
 * @see YYΩIO_printformat_generic_char
 */
YYIO_wur YYIO_nn() static inline
int yπio_printctx_put(yπio_printctx_t *t, const char str[], size_t str_len) {
	return YYΩIO_printformat_generic_char(t, str, str_len, false, false);
}

/**
 * @see YYΩIO_printformat_generic_char
 */
YYIO_wur YYIO_nn() static inline
int yπio_printctx_put_number(yπio_printctx_t *t, const char str[], size_t str_len, bool is_positive) {
	return YYΩIO_printformat_generic_char(t, str, str_len, true, is_positive);
}

#endif

/**
 * @}
 */

#endif /* YYIO_YIO_YΩIO_CTX_H_ */
