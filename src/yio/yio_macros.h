/**
 * @file yio_macros.h
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

#include "yio_macros_gen.h"

/* Basic config macros ------------------------------------------------------------------------------------------- */

// As of now (2020-03) eclipse doesn't understand _Generic, so disable it.
#ifdef __CDT_PARSER__
#define _Generic(a, ...)   _G(a)
#endif

/**
 * @def _yIO_GENERIC
 * _Generic replacement so that we can overload it in C++
 */
#ifdef __cplusplys
#error TODO: private _Generic alternative in C++ using typeid.type comparision \
		or just lambda dynamic dispatch
#define _yIO_GENERIC(TYPE, ...)
#else
#define _yIO_GENERIC(TYPE, ...)  _Generic(TYPE, _yIO_GENERIC_GEN(__VA_ARGS__))
#endif

/* basic macros ------------------------------------------------------------ */

/**
 * Is whatever you give a character string literal without a cast
 * https://en.cppreference.com/w/c/language/string_literal
 * @{
 */
#if 0
#define _yIO_IS_CHARACTER_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>2&&#x[0]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_UTF_8_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_16_BIT_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='u'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_32_BIT_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='U'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_WIDE_STRING_LITERAL_OLD(x) \
		(sizeof(#x)>3&&#x[0]=='L'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_STRING_LITERAL_OLD(x)(\
		sizeof(#x)>2&&#x[sizeof(#x)-2]=='"'&&(\
				#x[0]=='"'||(sizeof(#x)>3&&(#x[0]=='u'||#x[0]=='U'||#x[0]=='L')&&#x[1]=='"')\
				||\
				(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"')\
		)\
)
#endif

static inline
int _yIO__is_wide_string_literal(const char * const x, const size_t sizeof_x) {
	return sizeof_x > 3 && x[0] == 'L' && x[1] == '"' && x[sizeof_x - 2] == '"';
}
/**
 * Detect if @b x is a wide string literal.
 * @param x a wide string literal like L"abc" or anything else
 * @return nonzero if @b x is a wide string literal, anything else otherwise
 */
#define _yIO_IS_WIDE_STRING_LITERAL(x)  _yIO__is_wide_string_literal(#x, sizeof(#x))

static inline
int _yIO__is_string_literal(const char * const x, const size_t sizeof_x) {
	return sizeof_x > 2 && x[sizeof_x - 2] == '"' && (
					x[0] == '"' || (
							sizeof_x > 3 && (
									x[0] == 'u' ||
									x[0] == 'U' ||
									x[0] == 'L'
							) &&
							x[1] == '"'
					) || (
							sizeof_x > 4 &&
							x[0] == 'u' &&
							x[1] == '8' &&
							x[2] == '"'
					)
			);
}

/**
 * Detect if @b x is a string literal.
 * @param x Can be a string literal like u8"abc" or L"def" or anything else.
 * @return nonzero if @b x is any kind of string literal, anything else otherwise
 */
#define _yIO_IS_STRING_LITERAL(x)  _yIO__is_string_literal(#x, sizeof(#x))

/* Printers Generic Choosers ------------------------------------------------------------------------------------ */

/**
 * YIO_PRINT_ARGUMENTS
 * Create argument list for yprint functions
 * If an argument doesn't has 62 commas, then the function for it is chosen
 *    using _Generic expression with _yIO_PRINT_FUNC_GENERIC.
 * If an argument does has 62 commas, then the first argument that should be inside braces is extracted
 *   and the first argument from inside the braces is the function to handle the arguments.
 * The arguments are appended to the end of the function.
 *
 * For example a call like:
 *    yprint(1, yiocb(function, 2, 3));
 * Is expanded to:
 *    yprint((const _yIO_func_t[]){ _Generic((1), int: _yIO_print_int), function, NULL }, 1, 2, 3);
 *
 * The first argument is an array of functions to handle arguments, delimetered with NULL.
 * The rest of the arguments are unchanged and appended to ellipsis argument.
 */
#define YIO_PRINT_ARGUMENTS(...)  \
		_yIO_print_arguments_N(__VA_ARGS__, m4_seqcomma(m4_MLVLS, 0))(_yIO_PRINT_FUNC_GENERIC, __VA_ARGS__)

/**
 * @def YIO_SCAN_ARGUMENTS(...)
 * @param ... Scan arguments list.
 * Just like YIO_PRINT_ARGUMENTS.
 */
#define YIO_SCAN_ARGUMENTS(...)  \
		_yIO_scan_arguments_N(__VA_ARGS__, m4_seqcomma(m4_MLVLS, 0))(_yIO_SCAN_FUNC_GENERIC, __VA_ARGS__)

// _yIO_scan_arguments_N(__VA_ARGS__, m4_seqcommaX(1, 62, `2~), 1, 0)(_yIO_SCAN_FUNC_GENERIC, __VA_ARGS__)
