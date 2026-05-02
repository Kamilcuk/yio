/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_CTX_H_
#define YYIO_YIO_YIO_CTX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ctx_types.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup yio_printctx yio_printctx
 * @ingroup yio
 * @brief Printing context.
 * @{
 */

/* types ------------------------------------------------------------------------- */

/// Printing formatting options.
struct yio_printfmt_s {
	/// The field width. (uint8_t)-1 when unset. Values capped at 254.
	uint8_t width;
	/// The field precision. (uint8_t)-1 when unset. Values capped at 254.
	uint8_t precision;
	/// Filling character.
	char fill;
	/// May be one of '<' '>' '^' '=' characters or 0 when unset.
	char align;
	/// May be on of '+' '-' ' ' character or 0 when unset.
	char sign;
#if YIO_ENABLE_DIGIT_GROUPING
	/// May be set to 'L' or '_' ',' or 0 when unset.
	char grouping;
#endif
	/// The printing type character. The standard valid characters are "bcdueEfFgGnosxXp".
	char type;
	/// Use of alternate form.
	bool hash;
};

/// Get the width, falling back to a default if unset (0).
static inline size_t yio_width_get_default(uint8_t width, size_t default_val) {
	return width != 0 ? (size_t)width - 1 : default_val;
}

/// Get the precision, falling back to a default if unset (0).
static inline size_t yio_precision_get_default(uint8_t precision, size_t default_val) {
	return precision != 0 ? (size_t)precision - 1 : default_val;
}

/**
 * The structure that allows for printing context manipulation.
 */
struct YYIO_printctx_s {
	/// Current iterator in the format string.
	const char *fmt;
	/// va_list of current argument.
	va_list *va;
#if YIO_ENABLE_DYNAMIC_PFMT
	/// Copy of va_list when iterating
	va_list *startva;
#endif
	/// Iterator in callback functions.
	const yio_printdata_t *ifunc;
	/// The pointer to the data.
	const yio_printdata_t *startifunc;
	/// The outputting function.
	YYIO_printcb_t *out;
	/// User argument for outputting functions.
	void *outarg;
	/// The count of characters written.
	unsigned int writtencnt;
	/// Description of print formatting.
	struct yio_printfmt_s pf;
	/// How many arguments to skip when outputting current argument.
	unsigned char skip;
};

/* functions ------------------------------------------------------------------------- */

/**
 * For positional arguments, arm the _skip_do function for @c count jumps.
 * @param t
 * @param count The positional number.
 */
void YYIO_skip_arm(yio_printctx_t *t, unsigned int count);

/**
 * For positional arguments, skip until the proper positional argument is in va_arg.
 * @param t
 * @return 0 on success, otherwise error.
 */
int YYIO_skip_do(yio_printctx_t *t);

/**
 * Convert the string pointed to by ptr to a digit.
 * The first character has to be a digit already, so it can't fail.
 * @param ptr
 * @return The converted number.
 */
int YYIO_printctx_strtoi_noerr(const char **ptr);

/**
 * Parse the width or precision param, that can be either a number of a positional parameter.
 */
int YYIO_printctx_stdintparam(yio_printctx_t *t,
		const char *ptr, const char **endptr, uint8_t *res);

/**
 * Check if @c c is not nul and is one of characters in @c s.
 */
bool YYIO_strnulchrbool(const char *s, char c);

/**
 * Parse python formatting string.
 * @param c
 * @param pf
 * @param fmt Python formatting string like "{: < -0#123_.456}"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
YYIO_wur YYIO_nn()
int YYIO_pfmt_parse(yio_printctx_t *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr);

/* printctx ---------------------------------------------------- */

/**
 * This has to be the first function of a callback function.
 * Gets the next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type, see YYIO_IS_PROMOTED_TYPE
 * @def yio_printctx_va_arg(printctx, type)
 * @param printctx Printing context, pointer to yio_printctx_t
 * @param type Type of argument passed to va_list.
 * @return A value from the printctx va_list of type type.
 * @see yio_printctx_va_arg_promote
 */
#define yio_printctx_va_arg(printctx, type)   va_arg(*(printctx)->va, type)

#ifndef YIO_HAS_FLOATf
#error YIO_HAS_FLOATf not defined
#endif
#ifndef YIO_HAS_FLOATd
#error YIO_HAS_FLOATd not defined
#endif
#if YIO_HAS_FLOATf && YIO_HAS_FLOATd
/**
 * Automatically promote the type for integer types.
 * Argument has to be an arithmetic type, so that it can be promoted.
 * @def yio_printctx_va_arg_promote(printctx, numtype)
 * @param printctx Printing context, pointer to yio_printctx_t
 * @param numtype Numericall type, that arithmetics can be done for.
 * @return A value from the printctx va_list of type promoted numtype.
 * @see yio_printctx_va_arg
 */
#define yio_printctx_va_arg_promote(printctx, numtype)  \
		_Generic(+(numtype)1, \
			int: yio_printctx_va_arg(printctx, int), \
			unsigned int: yio_printctx_va_arg(printctx, unsigned int), \
			float: yio_printctx_va_arg(printctx, double), \
			default: yio_printctx_va_arg(printctx, numtype)  /* NOLINT(clang-diagnostic-varargs) */ \
		)
#else
#define yio_printctx_va_arg_promote(printctx, numtype)  \
		_Generic(+(numtype)1, \
			int: yio_printctx_va_arg(printctx, int), \
			unsigned int: yio_printctx_va_arg(printctx, unsigned int), \
			default: yio_printctx_va_arg(printctx, numtype)  /* NOLINT(clang-diagnostic-varargs) */ \
		)
#endif

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
int yio_printctx_init(yio_printctx_t *t);

/**
 * Write size count of bytes from ptr to output stream.
 * @param t
 * @param ptr
 * @param size
 * @return 0 on success, otherwise error.
 */
YYIO_wur YYIO_nn() YYIO_access_r(2, 3)
int yio_printctx_raw_write(yio_printctx_t *t, const char *ptr, size_t size);

/**
 * Calls the next context parsing function in chain.
 * To be used by formatting-setting functions.
 * @param t
 * @return The return value of the next formatting function.
 */
YYIO_wur YYIO_nn()
int yio_printctx_next(yio_printctx_t *t);

/**
 * Get print context formatting options.
 * @param t
 * @return A valid pointer.
 */
static inline
struct yio_printfmt_s *yio_printctx_get_fmt(yio_printctx_t *t) {
	return &t->pf;
}

/**
 * Internal callback called from
 * @see yio_printctx_print
 * @param t
 * @param data
 * @return
 */
YYIO_wur YYIO_nn(1, 2)
int YYIO_printctx_print_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, ...);

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... the printing parameters to print.
 * @return int 0 on success, otherwise error
 */
#define yio_printctx_printf(printctx, ...)  \
		YYIO_printctx_print_in(printctx, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

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
int YYIO_printformat_generic(yio_printctx_t *t,
		const char *str, size_t str_len, bool is_number, bool is_positive);

/**
 * From printing context output a string
 * @see YYIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yio_printctx_put(yio_printctx_t *t, const char *str, size_t str_len) {
	return YYIO_printformat_generic(t, str, str_len, false, false);
}

/**
 * From printing context output a number.
 * The @c str argument has to consist of only digits.
 * @see YYIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yio_printctx_put_number(yio_printctx_t *t, const char *str, size_t str_len, bool is_positive) {
	return YYIO_printformat_generic(t, str, str_len, true, is_positive);
}


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* YYIO_YIO_YIO_CTX_H_ */
