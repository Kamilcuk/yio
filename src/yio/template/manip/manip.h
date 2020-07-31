/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#include "../ctx.h"
#include "../../yio_config.h"
#include "slots.h"
#include "unhandled.h"
#include "print_bool.h"
#include "print_chars.h"

#if _yIO_TYPE_YIO
#include "print_float.h"
#include "print_int.h"
#include "print_modifiers.h"
#include "print_wchars.h"
#include "print_count.h"
#include "print_time.h"
#include "print_cfmt.h"
#include "scan_int.h"
#include "scan_string.h"
#include "scan_float.h"
#include "scan_modifiers.h"
#endif

/* ---------------------------------------------------------------- */


/* ---------------------------------------------------------------- */

/**
 * @def _yΩIO_PRINT_FUNC_GENERIC
 * For one argument choose the printing function dynamically using _Generic macro
 */
#if _yIO_TYPE_YIO
#define _yΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_INTS() \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS() \
				_yΩIO_PRINT_FUNC_GENERIC_FLOATS() \
				_yΩIO_PRINT_FUNC_GENERIC_WCHARS() \
				_yΩIO_PRINT_FUNC_GENERIC_BOOL() \
				_yΩIO_PRINT_GENERIC_TIME() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
				_yΩIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE() \
				_yΩIO_PRINT_FUNC_GENERIC_COUNT() \
		default: _yΩIO_print_unhadled_type \
		)))
#elif _yIO_TYPE_YWIO
#define _yΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_BOOL() \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
		default: _yΩIO_print_unhadled_type \
		)))
#elif _yIO_TYPE_YUIO
#define _yΩIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_BOOL() \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS() \
		default: _Generic((arg), \
				_yΩIO_PRINT_FUNC_GENERIC_CHARS_SECOND_STAGE() \
		default: _yΩIO_print_unhadled_type \
		)))
#else
#error
#endif

/* ---------------------------------------------------------------------- */

#if _yIO_TYPE_YIO
int _yIO_scan_until(yio_scanctx_t *t);
int _yIO_scan_until_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_except(yio_scanctx_t *t);
int _yIO_scan_except_charpntpnt(yio_scanctx_t *t);

/**
 * @defgroup yscan
 * @brief Scanners
 * @{
 */
/**
 * Scan until they are the characters in \c fmt string.
 * @param fmt string List of characters to scan until.
 * @param dest char* or char** Pointer to destination character.
 *             It may be a pointer to static buffer or characters,
 *             Or a pointer to pointer to char to use dynamic allocation.
 */
#define yscan_until(fmt, dest)   yiocb( \
		_Generic((dest), \
				Ychar *: _yIO_scan_until, \
				Ychar **:  _yIO_scan_until_charpntpnt \
		), \
		_Generic((fmt), Ychar *: (const Ychar *)(fmt), const Ychar *: (fmt)), \
		sizeof(dest)/sizeof(*dest), \
		_Generic((dest),Ychar *: (dest), Ychar(*)[sizeof(*dest)]: (dest), Ychar **: (dest)) \
)
/**
 * Scan all characters until they are not the character in \c fmt string.
 * @param fmt List of characters to exclude.
 * @param dest Pointer to destination string.
 *             It may be a pointer to static buffer or characters,
 *             Or a pointer to pointer to char to use dynamic allocation.
 */
#define yscan_except(fmt, dest)  yiocb( \
		_Generic((dest), \
				Ychar *: _yIO_scan_except, \
				Ychar **: _yIO_scan_except_charpntpnt \
		), \
		_Generic((fmt), Ychar *: (const Ychar *)(fmt), const Ychar *: (fmt)), \
		sizeof(dest)/sizeof(*dest), \
		_Generic((dest),Ychar *: (dest), Ychar(*)[sizeof(*dest)]: (dest), Ychar **: (dest)) \
)
/**
 * @}
 */
int _yIO_scan_char(yio_scanctx_t *t);
int _yIO_scan_schar(yio_scanctx_t *t);
int _yIO_scan_uchar(yio_scanctx_t *t);
int _yIO_scan_charpnt(yio_scanctx_t *t);
int _yIO_scan_constcharpnt(yio_scanctx_t *t);
int _yIO_scan_intpnt(yio_scanctx_t *t);
int _yIO_scan_wchar(yio_scanctx_t *t);
int _yIO_scan_wcharpnt(yio_scanctx_t *t);
int _yIO_scan_constwcharpnt(yio_scanctx_t *t);
int _yIO_scan_string_literal(yio_scanctx_t *t);
int _yIO_scan_string(yio_scanctx_t *t);
int _yIO_scan_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_const_char_array(yio_scanctx_t *t);
#endif

/**
 * @def _yIO_SCAN_FUNC_GENERIC
 * Choose the scanning function of argument using _Generic macro
 */
#if _yIO_TYPE_YIO
#define _yΩIO_SCAN_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_SCAN_FUNC_GENERIC_SLOTS() \
		default: _Generic((arg), \
				_yIO_SCAN_FUNC_GENERIC_INTS() \
				_yIO_SCAN_FUNC_GENERIC_FLOATS() \
				const char *: _yIO_scan_string_literal, \
				char *: (_yIO_IS_CHARACTER_STRING_LITERAL(arg) ? _yIO_scan_string_literal : _yIO_scan_string), \
				char **: _yIO_scan_charpntpnt, \
				char (*)[sizeof(*arg)]: (_yIO_IS_CHARACTER_STRING_LITERAL(arg) ? _yIO_scan_string_literal : _yIO_scan_string), \
				char (* const)[sizeof(*arg)]: (_yIO_IS_CHARACTER_STRING_LITERAL(arg) ? _yIO_scan_string_literal : _yIO_scan_string), \
				const char (*)[sizeof(*arg)]: _yIO_scan_const_char_array, \
				const char (* const)[sizeof(*arg)]: _yIO_scan_const_char_array, \
		default: _yΩIO_scan_unhandled_type \
		))
#elif _yIO_TYPE_YWIO
#define _yΩIO_SCAN_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_SCAN_FUNC_GENERIC_SLOTS() \
		default: _yΩIO_scan_unhandled_type \
		)
#elif _yIO_TYPE_YUIO
#define _yΩIO_SCAN_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				_yΩIO_SCAN_FUNC_GENERIC_SLOTS() \
		default: _yΩIO_scan_unhandled_type \
		)
#else
#error
#endif

