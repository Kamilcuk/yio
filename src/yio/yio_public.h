/**
 * @file yio_public.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio_config.h"
#include "yio_macros.h"
#include "yio_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* printfmt ---------------------------------------------------------------------------- */

/**
 * @defgroup printfmt Printing format
 * @{
 */

/// Enumeration of allowed align values
enum YIO_FMT_ALIGN_e {
	YIO_FMT_ALIGN_RIGHT = '<',
	YIO_FMT_ALIGN_CENTER = '^',
	YIO_FMT_ALIGN_LEFT = '>',
	YIO_FMT_ALIGN_SIGN = '=',
};

/// Enumeration of allowed sign values
enum YIO_FMT_SIGN_e {
	YIO_FMT_SIGN_BOTH = '+',
	YIO_FMT_SIGN_NEG = '-',
	YIO_FMT_SIGN_SPACE = ' ',
};

/// Enumeration of allowed hash values
enum YIO_FMT_HASH_e {
	YIO_FMT_HASH_NORMAL = 0,
	YIO_FMT_HASH_ALTERNATE = '#',
};

/// Enumeration of allowed grouping values
enum YIO_FMT_GROUPING_e {
	YIO_FMT_GROUPING_NONE = 0,
	YIO_FMT_GROUPING_UNDERSCORE = '_',
	YIO_FMT_GROUPING_COMMA = ',',
};

/// Printing formatting options.
struct yio_printfmt_s {
	/// The field width. Negative when unset.
	int width;
	/// The field precision. Negative when unset.
	int precision;
	/// Filling character.
	char fill;
	/// May be one of '<' '>' '^' '=' characters or 0 when unset.
	char align;
	/// May be on of '+' '-' ' ' character or 0 when unset.
	char sign;
	/// May be '#' or 0 when unset.
	char hash;
	/// May be set to '_' ',' or 0 when unset.
	char grouping;
	/// The printing type character. The standard valid characters are "bcdeEfFgGnosxX%".
	char type;
};
/**
 * @}
 */

/* ---------------------------------------------------------------------------- */

/**
 * @defgroup printctx Yio print context
 * @{
 */
/**
 * Print context.
 */
typedef struct _yIO_printctx_s yio_printctx_t;

/**
 * Write data to output stream.
 * @param arg Custom argument passed with callback registration.
 * @param data The pointer to the data to print.
 * @param count Count of characters to print
 * @return 0 on success, anything else on error
 */
typedef int _yIO_printcb_t(void *arg, const char *data, size_t count);

/**
 * The type of callback functions.
 * It is expected(!) that the first thing the function will do is read the arguments from va_list
 * Even if function fails, the `*va` pointer has to be updated with the arguments read.
 * This is the most important contract.
 * @param t Printing context.
 * @return 0 on success, anything else on error.
 */
typedef int (*_yIO_printfunc_t)(yio_printctx_t *t);

/**
 * Get's passed to generic printing functions as first argument.
 */
typedef struct _yIO_printdata_s {
	/**
	 * Format string or NULL.
	 */
	const char * const fmt;
	/**
	 * Array of printing function pointers pointers.
	 */
	const _yIO_printfunc_t * const funcs;
	/**
	 * Array of argument sizes for va_arg argument checking.
	 */
	const size_t * const argsizes;
	/**
	 * Array of stringified arguments.
	 */
	const char * const * const strargs;
} const yio_printdata_t;

/**
 * Write size count of bytes from ptr to output stream.
 * @param t
 * @param ptr
 * @param size
 * @return 0 on success, anything else on error.
 */
_yIO_wur _yIO_nn()
int yio_printctx_out(yio_printctx_t *t, const char *ptr, size_t size);

/**
 * Calls the next context parsing function in chain.
 * To be used by formatting-setting functions.
 * @param t
 * @return The return value of the next formatting function.
 */
_yIO_wur _yIO_nn()
int yio_printctx_next(yio_printctx_t *t);

/**
 * Get print context formatting options.
 * @param t
 * @return A valid pointer.
 */
_yIO_wur _yIO_nn() _yIO_const
struct yio_printfmt_s *yio_printctx_get_fmt(yio_printctx_t *t);

_yIO_wur _yIO_nn()
va_list *_yIO_printctx_inc_va_list(yio_printctx_t *t, size_t sizeof_realtype);

_yIO_wur _yIO_nn()
int _yIO_printctx_print(yio_printctx_t *t, yio_printdata_t *data, ...);

/**
 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * @param ... The printing parameters to print.
 * @return int 0 on success, anything else on error
 */
#define yio_printctx_print(printctx, ...)   \
		_yIO_printctx_print(printctx, YIO_PRINT_ARGUMENTS(NULL,__VA_ARGS__))
/**

 * Use it to print data from inside a printing context.
 * @param printctx Printing context to print to
 * param ... The format string followed by...
 * @param ... the printing parameters to print.
 * @return int 0 on success, anything else on error
 */
#define yio_printctx_printf(printctx, ...)  \
		_yIO_printctx_print(printctx, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Get next argument from variadic arguments stack. The argument has type `type`.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that `type` is not a promoted type, see _yIO_IS_PROMOTED_TYPE
 * @param printctx Printing context, pointer to yio_printctx_t
 * @param type Type of argument passed to va_list.
 * @return A value from the printcts va_list of type type.
 */
#define yio_printctx_va_arg(printctx, type)  yio_printctx_va_arg2(printctx, type, type)

/**
 * Get next argument of type 'realtype' from arguments stack.
 * The real argument type after implicit conversion is 'promotedtype'.
 * This function has only usage for 'realtype' equal to char, short or float.
 * @param printctx Printing context, pointer to yio_printctx_t
 * @param realtype The real type that was passed to va_list.
 * @param promotedtype The promoted type of real type.
 * @return A value from the printcts va_list of type promotedtype.
 */
#define yio_printctx_va_arg2(printctx, realtype, promotedtype)  \
		va_arg(*_yIO_printctx_inc_va_list(printctx, sizeof(realtype)), promotedtype)

/**
 * Automatically promote the type for integer types.
 * Argument has to be an arithmetic type, so that it can be promoted.
 * @param printctx Printing context, pointer to yio_printctx_t
 * @param numtype Numericall type, that arithmetics can be done for.
 * @return A value from the printcts va_list of type promoted numtype.
 */
#define yio_printctx_va_arg_num(printctx, numtype)  _Generic((numtype)1*1,\
		int: yio_printctx_va_arg2(printctx, numtype, int), \
		unsigned: yio_printctx_va_arg2(printctx, numtype, unsigned), \
		float: yio_printctx_va_arg2(printctx, numtype, double), \
default: yio_printctx_va_arg2(printctx, numtype, numtype) \
)
/**
 * @}
 */

/* --------------------------------------------------------------------------- */

/**
 * @defgroup scanfmt Yio scanning format
 * @{
 */
/**
 * Scanning formatting options.
 */
struct yio_scanfmt_s {
	/**
	 * Field width - how many character can you read from input.
	 * Negative means endless.
	 */
	int width;
	/**
	 * TODO: pointer to "[^abc]" or "[def]"
	 */
	const char *set;
	/**
	 * 0 or '*', when input should be ignored.
	 */
	char ignore;
	/**
	 * Scanning type, standard valid characters are "iduoxfegacspn".
	 */
	char type;
};
/**
 * @}
 */

/**
 * @defgroup scanctx Yio scanning context
 * @{
 */
/**
 * Forward declaration of yio_scanctx_t.
 * This is an opaque structure not visible to users.
 * Use yio_scanctx_* functions to access it.
 */
typedef struct _yIO_scanctx_s yio_scanctx_t;

/**
 * Scanner function.
 * @param t Pointer to scanning context.
 * @return 0 on success, anything else on error.
 */
typedef int (*_yIO_scanfunc_t)(yio_scanctx_t *t);

/**
 * Read one byte from the input.
 * @param arg Custom user context.
 * @param data A valid character or EOF.
 * @return 0 on success, anything else on error
 */
typedef int _yIO_scancb_t(void *arg, int *data);

/**
 * A list of arguments created by macro substitution.
 */
typedef const struct _yIO_scandata_s {
	/**
	 * Format string or NULL.
	 */
	const char * const fmt;
	/**
	 * Array of scanning function pointers pointers.
	 */
	const _yIO_scanfunc_t * const funcs;
	/**
	 * Array of destination memory sizes.
	 */
	const size_t * const argpntsizes;
	/**
	 * Array of argument sizes for va_arg argument checking.
	 */
	const size_t * const argsizes;
	/**
	 * Array of stringified arguments.
	 */
	const char * const * const strargs;
} yio_scandata_t;

/**
 * Scanning result.
 * All y*scan(f)? functions return this.
 */
struct yio_scanret_s {
	/**
	 * YIO_ERROR_* number or 0 on success.
	 */
	int error;
	/**
	 * Count of items successfully scanned.
	 */
	unsigned nitem;
	/**
	 * Count of characters that were read,
	 * including the lastchar.
	 */
	size_t count;
	/**
	 * Last character that was read.
	 * When all input was read, this will be EOF.
	 */
	int lastchar;
};

/**
 * Read one character from input and save it in c.
 * @param t
 * @param c
 * @return 0 on success, otherwise error.
 */
_yIO_wur _yIO_nn()
int yio_scanctx_in(yio_scanctx_t *t, int *c);

/**
 * Does the same as ungetc.
 * The next call to yio_scanctx_in will re-return last character.
 * Should be called as the last function when scanning ended.
 * @param t
 */
_yIO_nn()
void yio_scanctx_unin(yio_scanctx_t *t);

/**
 * Call the next scanning function.
 * Should be called by formatting functions.
 * @param t
 * @return The return value of the next scanning function.
 */
_yIO_wur _yIO_nn()
int yio_scanctx_next(yio_scanctx_t *t);

/**
 * Get formatting options.
 * @param t
 * @return The pointer to scanning formatting options.
 */
_yIO_wur _yIO_nn() _yIO_const
struct yio_scanfmt_s *yio_scanctx_get_fmt(yio_scanctx_t *t);

/**
 * Get the buffer destination size of the current argument.
 * When for example <tt>char pnt[20]; yscan(pnt);</tt>
 * Then this function will return <tt>20</tt>.
 * @param t
 * @return
 */
_yIO_wur _yIO_nn()
size_t yio_scanctx_arg_size(yio_scanctx_t *t);

/**
 * Increment to the next element in va_list.
 * @param t
 * @param sizeof_realtype The sizeof of the real type in va_list,
 *        for safety pseudo-type checking.
 * @return Pointer to va_list.
 */
_yIO_wur _yIO_nn()
va_list *_yIO_scanctx_inc_va_list(yio_scanctx_t *t, size_t sizeof_realtype);

_yIO_wur _yIO_nn()
int _yIO_scanctx_scan(yio_scanctx_t *t, yio_scandata_t *data, ...);

#define yio_scanctx_scan(scanctx, ...)   _yIO_scanctx_scan(scanctx, YIO_SCAN_ARGUMENTS(NULL,__VA_ARGS__))
#define yio_scanctx_scanf(scanctx, ...)  _yIO_scanctx_scan(scanctx, YIO_SCAN_ARGUMENTS(__VA_ARGS__))

/**
 * @def yio_scanctx_va_arg(scanctx, type)
 * @param scanctx Context to yio_scanctx_t
 * @param type Type
 * @return A variable of passed type from va_list.
 * Get next argument from variadic arguments stack. The argument has type @c type.
 * The type argument undergoes implicit conversion when calling a variadic function,
 * so char, short is converted to int, float is converted to double.
 * If it errors on you, that means that @c type is not a promoted type,
 * see #_yIO_IS_PROMOTED_TYPE
 */
#define yio_scanctx_va_arg(scanctx, type)  \
		va_arg(*_yIO_scanctx_inc_va_list(scanctx, sizeof(type)), type)
/**
 * @}
 */

/* print -------------------------------------------------------------------- */

int _yIO_print_char(yio_printctx_t *t);
int _yIO_print_schar(yio_printctx_t *t);
int _yIO_print_uchar(yio_printctx_t *t);
int _yIO_print_short(yio_printctx_t *t);
int _yIO_print_ushort(yio_printctx_t *t);
int _yIO_print_int(yio_printctx_t *t);
int _yIO_print_uint(yio_printctx_t *t);
int _yIO_print_long(yio_printctx_t *t);
int _yIO_print_ulong(yio_printctx_t *t);
int _yIO_print_llong(yio_printctx_t *t);
int _yIO_print_ullong(yio_printctx_t *t);
int _yIO_print_charpnt(yio_printctx_t *t);
int _yIO_print_constcharpnt(yio_printctx_t *t);
int _yIO_print_intpnt(yio_printctx_t *t);
int _yIO_print_wchar(yio_printctx_t *t);
int _yIO_print_wcharpnt(yio_printctx_t *t);
int _yIO_print_constwcharpnt(yio_printctx_t *t);
int _yIO_print_float(yio_printctx_t *t);
int _yIO_print_double(yio_printctx_t *t);
int _yIO_print_ldouble(yio_printctx_t *t);
int _yIO_print_wrong_type(yio_printctx_t *t);
int _yIO_print_floatpnt(yio_printctx_t *t);
int _yIO_print__Float16(yio_printctx_t *t);
int _yIO_print__Float32(yio_printctx_t *t);
int _yIO_print__Float64(yio_printctx_t *t);
int _yIO_print__Float80(yio_printctx_t *t);
int _yIO_print__Float128(yio_printctx_t *t);
int _yIO_print__Float32x(yio_printctx_t *t);
int _yIO_print__Float64x(yio_printctx_t *t);
int _yIO_print__Float128x(yio_printctx_t *t);
int _yIO_print__Decimal32(yio_printctx_t *t);
int _yIO_print__Decimal64(yio_printctx_t *t);
int _yIO_print__Decimal128(yio_printctx_t *t);
int _yIO_print__Decimal32x(yio_printctx_t *t);
int _yIO_print__Decimal64x(yio_printctx_t *t);
int _yIO_print__Decimal128x(yio_printctx_t *t);
int _yIO_print___int128(yio_printctx_t *t);
int _yIO_print_u__int128(yio_printctx_t *t);

int _yIO_print_right(yio_printctx_t *t);
int _yIO_print_noright(yio_printctx_t *t);
int _yIO_print_left(yio_printctx_t *t);
int _yIO_print_center(yio_printctx_t *t);
int _yIO_print_fixed(yio_printctx_t *t);
int _yIO_print_scientific(yio_printctx_t *t);
int _yIO_print_hexfloat(yio_printctx_t *t);
int _yIO_print_persistent(yio_printctx_t *t);
int _yIO_print_nopersistent(yio_printctx_t *t);
int _yIO_print_precision(yio_printctx_t *t);
int _yIO_print_width(yio_printctx_t *t);
int _yIO_print_unitbuf(yio_printctx_t *t);
int _yIO_print_nounitbuf(yio_printctx_t *t);
int _yIO_print_cfmt(yio_printctx_t *t);
int _yIO_print_pfmt(yio_printctx_t *t);

_yIO_nn(1, 2)
int _yIO_ybprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, ...);
_yIO_nn(1)
int _yIO_yprintf(yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yIO_yfprintf(FILE *file, yio_printdata_t *data, ...);
_yIO_nn(1, 3)
int _yIO_ysprintf(char *dest, size_t size, yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yIO_yaprintf(char **strp, yio_printdata_t *data, ...);
_yIO_nn(1, 2)
int _yIO_yreaprintf(char **strp, yio_printdata_t *data, ...);
_yIO_nn(1) _yIO_malloc _yIO_wur
		char *_yIO_yformatf(yio_printdata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
		char *_yIO_yreformatf(char *str, yio_printdata_t *data, ...);

/* Private Scan Symbols ------------------------------------------------------ */

int _yIO_scan_char(yio_scanctx_t *t);
int _yIO_scan_schar(yio_scanctx_t *t);
int _yIO_scan_uchar(yio_scanctx_t *t);
int _yIO_scan_short(yio_scanctx_t *t);
int _yIO_scan_ushort(yio_scanctx_t *t);
int _yIO_scan_int(yio_scanctx_t *t);
int _yIO_scan_uint(yio_scanctx_t *t);
int _yIO_scan_long(yio_scanctx_t *t);
int _yIO_scan_ulong(yio_scanctx_t *t);
int _yIO_scan_llong(yio_scanctx_t *t);
int _yIO_scan_ullong(yio_scanctx_t *t);
int _yIO_scan_charpnt(yio_scanctx_t *t);
int _yIO_scan_constcharpnt(yio_scanctx_t *t);
int _yIO_scan_intpnt(yio_scanctx_t *t);
int _yIO_scan_wchar(yio_scanctx_t *t);
int _yIO_scan_wcharpnt(yio_scanctx_t *t);
int _yIO_scan_constwcharpnt(yio_scanctx_t *t);
int _yIO_scan_float(yio_scanctx_t *t);
int _yIO_scan_double(yio_scanctx_t *t);
int _yIO_scan_ldouble(yio_scanctx_t *t);
int _yIO_scan_wrong_type(yio_scanctx_t *t);
int _yIO_scan_floatpnt(yio_scanctx_t *t);
int _yIO_scan_until(yio_scanctx_t *t);
int _yIO_scan_except(yio_scanctx_t *t);
int _yIO_scan_except_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_string_literal(yio_scanctx_t *t);
int _yIO_scan_string(yio_scanctx_t *t);
int _yIO_scan_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_const_char_array(yio_scanctx_t *t);

int _yIO_scan_ignore(yio_scanctx_t *t);
int _yIO_scan_width_int(yio_scanctx_t *t);
int _yIO_scan_width_size_t(yio_scanctx_t *t);

_yIO_nn(1, 3) _yIO_wur
struct yio_scanret_s _yIO_ybscanf(_yIO_scancb_t *in, void *arg, yio_scandata_t *data, ...);
_yIO_nn(1) _yIO_wur
struct yio_scanret_s _yIO_yscanf(yio_scandata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
struct yio_scanret_s _yIO_yfscanf(FILE *file, yio_scandata_t *data, ...);
_yIO_nn(1, 2) _yIO_wur
struct yio_scanret_s _yIO_ysscanf(char *src, yio_scandata_t *data, ...);



