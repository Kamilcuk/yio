/**
 * @file manip.h
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config();
#pragma once
#include "yio/yio/ctx_public.h"
#include "yio/yio_config.h"

/* ---------------------------------------------------------------- */

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
int _yIO_print_pfmt(yio_printctx_t *t);
int _yIO_print_cfmt(yio_printctx_t *t);

/**
 * @defgroup yp__mod Printing modifiers.
 * @{
 **/
/// Align output to the right.
#define ypright()         yiocb(_yIO_print_right)
/// Align output to the left.
#define ypleft()          yiocb(_yIO_print_left)
/// Center the output within width.
#define ypcenter()        yiocb(_yIO_print_center)
/// Outputs the next float in fixed format.
#define ypfixed()         yiocb(_yIO_print_fixed)
/// Outputs the next float in scientific format.
#define ypscientific()    yiocb(_yIO_print_scientific)
/// Print floats using hexadecimal notation.
#define yphexfloat()      yiocb(_yIO_print_hexfloat)
/// Set formatting precision.
#define ypprecision(arg)  yiocb(_yIO_print_precision, _yIO_CAST_TO_INT(arg))
/// Set field width.
#define ypwidth(arg)      yiocb(_yIO_print_width, _yIO_CAST_TO_INT(arg))
/// Print the unit.
#define ypunitbuf()       yiocb(_yIO_print_unitbuf)
/// Do not print the unit.
#define ypnounitbuf()     yiocb(_yIO_print_nounitbuf)

/**
 * Format output by reading python format string.
 * @def yppfmt(...)
 * @param ... Python formatting string like "{:<20}".
 * @param ... Followed by optional up to two int values.
 */
#define _yIO_yppfmt_0(fmt)               fmt
#define _yIO_yppfmt_1(fmt, spec)         fmt, _Generic((spec),int:(spec))
#define _yIO_yppfmt_2(fmt, spec, spec2)  fmt, _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yIO_yppfmt_N(_0,_1,_2,N,...)  _yIO_yppfmt_##N
#define yppfmt(...)  \
		yiocb(_yIO_print_pfmt, _yIO_yppfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * Format output by reading C format string.
 * @def ypcfmt(...)
 * @param ... C formatting string like "%s".
 * @param ... Optional up to two int values.
 */
#define _yIO_ypcfmt_0(fmt)                fmt
#define _yIO_ypcfmt_1(fmt, spec)          fmt, _Generic((spec),int:(spec))
#define _yIO_ypcfmt_2(fmt, spec, spec2)   fmt, _Generic((spec),int:(spec)), _Generic((spec2),int:(spec2))
#define _yIO_ypcfmt_N(_0,_1,_2,N,...)  _yIO_ypcfmt_##N
#define ypcfmt(...)  \
		yiocb(_yIO_print_cfmt, _yIO_ypcfmt_N(__VA_ARGS__,2,1,0)(__VA_ARGS__))

/**
 * @}
 */

int _yIO_print_wrong_type(yio_printctx_t *t);
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
int _yIO_print_floatpnt(yio_printctx_t *t);
int _yIO_print___int128(yio_printctx_t *t);
int _yIO_print_u__int128(yio_printctx_t *t);

/**
 * @defgroup yp__ Printing overloads.
 * @{
 */
/// Print wchar_t character.
#define ypwchar(wchar)         yiocb(_yIO_print_wchar, wchar)
/// Print a string of wchar_t characters.
#define ypwstrint(wstring)     yiocb(_yIO_print_wstring, wstring)
/**
 * @}
 */

/**
 * For one argument choose the printing function dynamically using _Generic macro
 */
#define _yIO_PRINT_FUNC_GENERIC(arg, ...) \
		_Generic((arg), \
				char: _yIO_print_char, \
				signed char: _yIO_print_schar, \
				unsigned char: _yIO_print_uchar, \
				short: _yIO_print_short, \
				unsigned short: _yIO_print_ushort, \
				int: _yIO_print_int, \
				unsigned int: _yIO_print_uint, \
				long: _yIO_print_long, \
				unsigned long: _yIO_print_ulong, \
				long long: _yIO_print_llong, \
				unsigned long long: _yIO_print_ullong, \
				char *: _yIO_print_charpnt, \
				const char *: _yIO_print_constcharpnt, \
				wchar_t *: _yIO_print_wcharpnt, \
				const wchar_t *: _yIO_print_constwcharpnt, \
				_yIO_PRINT_FUNC_GENERIC_FLOATS() \
				_yIO_PRINT_SLOTS() \
		default: \
		_Generic((arg), \
				wchar_t: _yIO_print_wchar, \
				int *: _yIO_print_intpnt, \
				float *: _yIO_print_floatpnt, \
		default: _yIO_print_wrong_type \
		)\
)

/* ---------------------------------------------------------------------- */

int _yIO_scan_until(yio_scanctx_t *t);
int _yIO_scan_until_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_except(yio_scanctx_t *t);
int _yIO_scan_except_charpntpnt(yio_scanctx_t *t);

/**
 * @defgroup yscan Scanners.
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


int _yIO_scan_ignore(yio_scanctx_t *t);
int _yIO_scan_width_int(yio_scanctx_t *t);
int _yIO_scan_width_size_t(yio_scanctx_t *t);

/**
 * @defgroup ys_mod Scanning modifiers.
 * @{
 */
/// Ignore next input.
#define ysignore()         yiocb(_yIO_scan_ignore)
/// Set the maximum field width. Negative disables it.
#define yswidth(width)  \
		yiocb(_Generic((width), \
				int: _yIO_scan_width_int, \
				size_t: _yIO_scan_width_size_t \
		), width)
/**
 * @}
 */

int _yIO_scan_wrong_type(yio_scanctx_t *t);
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
int _yIO_scan_floatpnt(yio_scanctx_t *t);
int _yIO_scan_string_literal(yio_scanctx_t *t);
int _yIO_scan_string(yio_scanctx_t *t);
int _yIO_scan_charpntpnt(yio_scanctx_t *t);
int _yIO_scan_const_char_array(yio_scanctx_t *t);

/**
 * Choose the scanning function of argument using _Generic macro
 */
#define _yIO_SCAN_FUNC_GENERIC(arg, ...)  _Generic((arg), \
		short *: _yIO_scan_short, \
		unsigned short *: _yIO_scan_ushort, \
		int *: _yIO_scan_int, \
		unsigned int *: _yIO_scan_uint, \
		long *: _yIO_scan_long, \
		unsigned long *: _yIO_scan_ulong, \
		long long *: _yIO_scan_llong, \
		unsigned long long *: _yIO_scan_ullong, \
		const char *: _yIO_scan_string_literal, \
		char *: (_yIO_IS_STRING_LITERAL(arg) ? \
						_yIO_scan_string_literal : \
						_yIO_scan_string), \
		char **: _yIO_scan_charpntpnt, \
		char (*)[sizeof(*arg)]: (_yIO_IS_STRING_LITERAL(arg) ? \
						_yIO_scan_string_literal : \
						_yIO_scan_string), \
		char (* const)[sizeof(*arg)]: (_yIO_IS_STRING_LITERAL(arg) ? \
						_yIO_scan_string_literal : \
						_yIO_scan_string), \
		const char (*)[sizeof(*arg)]: _yIO_scan_const_char_array, \
		_yIO_SCAN_FUNC_GENERIC_FLOATS() \
		_yIO_SCAN_SLOTS() \
		const char (* const)[sizeof(*arg)]: _yIO_scan_const_char_array \
)

/* floats generated -------------------------------------------------------------- */

#ifndef __CDT_PARSER__

m4_include(`floatlist.m4~) // (type, name, suffix, strfromx, strtox)
m4_define(`m4_FUNC_GENERIC_GEN~,`
#ifdef _yIO_HAVE_$3
int _yIO_print_$2(yio_printctx_t *t);
int _yIO_scan_$2(yio_scanctx_t *t);
#define _yIO_PRINT_FUNC_GENERIC_$2()  $1: _yIO_print_$2,
#define _yIO_SCAN_FUNC_GENERIC_$2()  $1*: _yIO_scan_$2,
#else
#define _yIO_PRINT_FUNC_GENERIC_$2()
#define _yIO_SCAN_FUNC_GENERIC_$2()
#endif
~)m4_dnl;
m4_applyforeach(`m4_FUNC_GENERIC_GEN~, m4_floatlist)m4_dnl;

m4_define(`m4_PRINT_FUNC_GENERIC_FLOATS~, `_yIO_PRINT_FUNC_GENERIC_$2()~)m4_dnl;
#define _yIO_PRINT_FUNC_GENERIC_FLOATS() \
		m4_applyforeach(`m4_PRINT_FUNC_GENERIC_FLOATS~, m4_floatlist)

m4_define(`m4_SCAN_FUNC_GENERIC_FLOATS~, `_yIO_SCAN_FUNC_GENERIC_$2()~)m4_dnl;
#define _yIO_SCAN_FUNC_GENERIC_FLOATS() \
		m4_applyforeach(`m4_SCAN_FUNC_GENERIC_FLOATS~, m4_floatlist)

#endif

/* slots ---------------------------------------------------------------- */

#ifndef __CDT_PARSER__

m4_forloopX(100, m4_SLOTS_END, `
#ifndef YIO_PRINT_SLOT_`~X
#define YIO_PRINT_SLOT_`~X`~()
#endif
#ifndef YIO_SCAN_SLOT_`~X
#define YIO_SCAN_SLOT_`~X`~()
#endif
~)m4_dnl;

#endif

#define _yIO_PRINT_SLOTS()  m4_forloopX(100, m4_SLOTS_END, `YIO_PRINT_SLOT_`~X`~()~)

#define _yIO_SCAN_SLOTS()   m4_forloopX(100, m4_SLOTS_END, `YIO_SCAN_SLOT_`~X`~()~)

