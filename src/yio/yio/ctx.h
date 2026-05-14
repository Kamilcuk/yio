/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_CTX_H_
#define YIO_YIO_YIO_CTX_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "ctx_types.h"
#include "yio/yio_error.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
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
	/// The field width. 0 when unset. Values capped at UINT16_MAX-1.
	uint16_t width;
	/// The field precision. 0 when unset. Values capped at UINT16_MAX-1.
	uint16_t precision;
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

static const struct yio_printfmt_s YIO_printfmt_zero = {0};

/// Get the width, falling back to a default if unset (0).
static inline size_t yio_width_get_default(uint16_t width, size_t default_val) {
	return width != 0 ? (size_t)width - 1 : default_val;
}

/// Get the precision, falling back to a default if unset (0).
static inline size_t yio_precision_get_default(uint16_t precision, size_t default_val) {
	return precision != 0 ? (size_t)precision - 1 : default_val;
}

/**
 * The structure that allows for printing context manipulation.
 */
struct YIO_printctx_s {
	/// Current iterator in the format string.
	const char * __null_terminated fmt;
	/// va_list of current argument.
	va_list * __single va;
	/// Iterator in callback functions.
	const yio_printdata_t * __null_terminated ifunc;
#if YIO_ENABLE_DYNAMIC_PFMT
	/// Copy of va_list when iterating
	va_list * __single startva;
	/// The pointer to the data.
	const yio_printdata_t * __null_terminated startifunc;
#endif
	/// The outputting function.
	YIO_printcb_t * __single out;
	/// User argument for outputting functions.
	void * __single outarg;
	/// The count of characters written.
	unsigned int writtencnt;
	/// Description of print formatting.
	struct yio_printfmt_s pf;
#if YIO_ENABLE_DYNAMIC_PFMT
	/// How many arguments to skip when outputting current argument.
	unsigned char skip;
#endif
};

/* functions ------------------------------------------------------------------------- */

#if YIO_ENABLE_DYNAMIC_PFMT
typedef struct {
	const yio_printdata_t __null_terminated *saveifunc;
	va_list *saveva;
	va_list va;
} YIO_skipper;

static int YIO_skip_do_in(YIO_skipper *s, yio_printctx_t *t, unsigned char count) {
	// Setup
	assert(s->saveifunc == NULL);
	s->saveva = t->va;
	s->saveifunc = t->ifunc;
	t->va = &s->va;
	t->ifunc = t->startifunc;
	t->skip = count;
	//
	const char *savefmt = t->fmt;
	t->fmt = NULL;
	while (t->skip) {
		if (*t->ifunc == NULL) return YIO_ERROR_TOO_MANY_FMT;
		const int err = (*t->ifunc++)(t);
		if (err != YIO_ERROR_SKIPPING) return err;
	}
	t->fmt = savefmt;
	return 0;
}
static bool YIO_skip_end_in(YIO_skipper *s, yio_printctx_t *t) {
	if (s->saveifunc) {
		t->ifunc = s->saveifunc;
		t->va = s->saveva;
		s->saveifunc = 0;
		return true;
  }
	return false;
}
#define YIO_skipper_do(s, t, count)  (va_copy((s)->va, *(t)->startva), YIO_skip_do_in(s, t, count))
#define YIO_skipper_end(s, t) do { if (YIO_skip_end_in(s, t)) va_end((s)->va); } while(0)
#else
#define YIO_skipper_setup(s, t, count) /* */
#define YIO_skipper_end(s, t) /* */
#endif

/**
 * Convert the string pointed to by ptr to a digit.
 * The first character has to be a digit already, so it can't fail.
 * @param ptr
 * @return The converted number.
 */
unsigned int YIO_printctx_strtou_noerr(const char **ptr);

/**
 * Parse the width or precision param, that can be either a number of a positional parameter.
 */
int YIO_printctx_stdintparam(yio_printctx_t *t, const char *ptr, const char **endptr, uint16_t *res);

/**
 * Parse python formatting string.
 * @param c
 * @param pf
 * @param fmt Python formatting string like "{: < -0#123_.456}"
 * @param endptr Will be set to the last character parsed in fmt
 * @return 0 on success, otherwise error
 */
YIO_wur YIO_nn()
int YIO_pfmt_parse(yio_printctx_t *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr);

/* printctx ---------------------------------------------------- */

/**
 * This has to be the first function of a callback function.
 * Gets the next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type, see YIO_IS_PROMOTED_TYPE
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

static const uint16_t YIO_PRECISION_MAX = UINT16_MAX - 1;

YIO_wur YIO_nn() static inline int YIO_printctx_init_in(yio_printctx_t *t) {
#if YIO_ENABLE_DYNAMIC_PFMT
	if (t->skip) {
		t->skip--;
		return YIO_ERROR(YIO_ERROR_SKIPPING, "error part of skipping arguments when iterating over them");
	}
#endif
	return t->fmt ? YIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt) : 0;
}

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
YIO_wur YIO_nn() static inline int yio_printctx_init(yio_printctx_t *t) {
	if (t->out == NULL) {
		return YIO_ERROR(YIO_ERROR_DYNAMIC_NOT_NUMBER, "dynamic width or precision must be an integer");
	}
	return YIO_printctx_init_in(t);
}

/**
 * Similar to @c yio_printctx_init, but if @c t->out is NULL, it will store @c val
 * in @c t->pf.precision (mapped as val+1) and return @c YIO_PRINTCTX_INIT_OR_NUMBER_MAGIC.
 */
static inline int yio_printctx_init_or_number(yio_printctx_t *t, int val) {
	if (t->out == NULL) {
		if (val < 0) {
			return YIO_ERROR(YIO_ERROR_DYNAMIC_NEGATIVE, "dynamic width or precision cannot be negative");
		}
		t->pf.precision = (uint16_t)(val >= (int)YIO_PRECISION_MAX ? YIO_PRECISION_MAX : (uint16_t)val) + 1;
		return YIO_ERROR(YIO_ERROR_GOT_DYNAMIC_VALUE, "dynamic parameter consumed");
	}
	return YIO_printctx_init_in(t);
}

static inline int yio_printctx_err_skip(yio_printctx_t *t, int err, unsigned count) {
	(void)t;
	(void)count;
#if YIO_ENABLE_DYNAMIC_PFMT
	if (err == YIO_ERROR_SKIPPING) {
		// This requires sub-skipping, where the child function also is skipping.
		t->skip += count;
	}
#endif
	return err;
}

/**
 * Write size count of bytes from ptr to output stream.
 * @param t
 * @param ptr
 * @param size
 * @return 0 on success, otherwise error.
 */
YIO_wur YIO_nn() YIO_access_r(2, 3)
int yio_printctx_raw_write(yio_printctx_t *t, const char *ptr, size_t size);

/**
 * Calls the next context parsing function in chain.
 * To be used by formatting-setting functions.
 * @param t
 * @return The return value of the next formatting function.
 */
YIO_wur YIO_nn()
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
YIO_wur YIO_nn(1, 2, 4)
int YIO_printctx_vprint_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, va_list *va);

/**
 * Internal callback called from
 * @see yio_printctx_print
 * @param t
 * @param data
 * @return
 */
YIO_wur YIO_nn(1, 2)
int YIO_printctx_print_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, ...);

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... the printing parameters to print.
 * @return int 0 on success, otherwise error
 */
#define yio_printctx_printf(printctx, ...)  \
		YIO_printctx_print_in(printctx, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Generic formatter to output stuff formatted according to python standard format specification.
 * @param t
 * @param str The string to output.
 * @param str_len Length of @c str.
 * @param is_number Is @c str a string of a number, consisting of only digits?
 * @param is_positive If @c str is a number, is it a positive or negative number?
 * @return 0 on success, otherwise error.
 */
YIO_wur YIO_nn()
int YIO_printformat_generic(yio_printctx_t *t,
		const char *str, size_t str_len, bool is_number, bool is_positive);

/**
 * From printing context output a string
 * @see YIO_printformat_generic
 */
YIO_wur YIO_nn() static inline
int yio_printctx_put(yio_printctx_t *t, const char *str, size_t str_len) {
	return YIO_printformat_generic(t, str, str_len, false, false);
}

/**
 * From printing context output a number.
 * The @c str argument has to consist of only digits.
 * @see YIO_printformat_generic
 */
YIO_wur YIO_nn() static inline
int yio_printctx_put_number(yio_printctx_t *t, const char *str, size_t str_len, bool is_positive) {
	return YIO_printformat_generic(t, str, str_len, true, is_positive);
}


/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_CTX_H_
