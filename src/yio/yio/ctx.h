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
#include "../yio_config.h"
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
	/// May be set to 'L' or '_' ',' or 0 when unset.
	char grouping;
	/// The printing type character. The standard valid characters are "bcdeEfFgGnosxXp".
	char type;
	/// The conversion specifier. Do not mix with type!
	char c_onversion;
	/// Use of alternate form.
	bool hash;
};

/**
 * Check if width is set in the formatting options.
 * @param width The width value.
 * @return true if width is set, false if it is unset.
 */
static inline bool yio_width_isset(uint8_t width) {
	return width != (uint8_t)-1;
}

/**
 * Check if precision is set in the formatting options.
 * @param precision The precision value.
 * @return true if precision is set, false if it is unset.
 */
static inline bool yio_precision_isset(uint8_t precision) {
	return precision != (uint8_t)-1;
}

/**
 * The default values of printfmt.
 */
static const struct yio_printfmt_s YYIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = ' ',
		.sign = '-',
};

/**
 * Print context.
 */
typedef struct YYIO_printctx_s yio_printctx_t;

/**
 * The type representing a callback that will write output data to the user specified place.
 * @param arg Custom argument passed with callback registration.
 * @param data The pointer to the data to print.
 * @param count Count of characters to print
 * @return 0 on success, otherwise error
 */
typedef int YYIO_printcb_t(void *arg, const char *data, size_t count)
		YYIO_wur YYIO_nn(2) YYIO_access_r(2, 3);

/**
 * The type of callback functions, but abstractly, this represents
 * the "printing data" that are needed to represent the printing context.
 * Callback functions need to follow a contract, in order:
 * 1. Firstly, they need to eat the argument using @c yio_printctx_va_arg.
 * 2. Optionally, then they need to parse the format string up until a '}'.
 * 3. Then, they need to call @c yio_printctx_init.
 * 4. If _init returns non-zero, the value has to be returned.
 * 5. Then, there is a place for custom handling.
 * 6. The function should call @c yio_printctx_put* functions and return with it's return value.
 * This contact is really important for making positional arguments work properly.
 * @param t Printing context.
 * @return 0 on success, otherwise error.
 */
typedef int (*const yio_printdata_t)(yio_printctx_t *t);

/**
 * The structure that allows for printing context manipulation.
 */
struct YYIO_printctx_s {
	/// Current iterator in the format string.
	const char *fmt;
	/// va_list of current argument.
	va_list *va;
	/// Copy of va_list when iterating
	va_list *startva;
	/// Iterator in callback functions.
	yio_printdata_t *ifunc;
	/// The pointer to the data.
	yio_printdata_t *startifunc;
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
int YYIO_printctx_print_in(yio_printctx_t *t, yio_printdata_t *data, const char *fmt, ...);

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
		const char str[], size_t str_len, bool is_number, bool is_positive);

/**
 * From printing context output a string
 * @see YYIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yio_printctx_put(yio_printctx_t *t, const char str[], size_t str_len) {
	return YYIO_printformat_generic(t, str, str_len, false, false);
}

/**
 * From printing context output a number.
 * The @c str argument has to consist of only digits.
 * @see YYIO_printformat_generic
 */
YYIO_wur YYIO_nn() static inline
int yio_printctx_put_number(yio_printctx_t *t, const char str[], size_t str_len, bool is_positive) {
	return YYIO_printformat_generic(t, str, str_len, true, is_positive);
}


/**
 * @}
 */

#endif /* YYIO_YIO_YIO_CTX_H_ */
