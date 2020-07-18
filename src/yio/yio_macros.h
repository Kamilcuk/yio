/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config() m4_dnl;
#pragma once

#include "yio_macros_gen.h"

/* Basic config macros ------------------------------------------------------------------------------------------- */

// As of now (2020-03) eclipse doesn't understand _Generic, so disable it.
#ifdef __CDT_PARSER__
#define _Generic(a, ...)   _G(a)
#endif

/* basic macros ------------------------------------------------------------ */

/**
 * Is whatever you give a character string literal without a cast
 * https://en.cppreference.com/w/c/language/string_literal
 * @{
 */
#define _yIO_IS_CHARACTER_STRING_LITERAL(x) \
		(sizeof(#x)>2&&#x[0]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_UTF_8_STRING_LITERAL(x) \
		(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_16_BIT_WIDE_STRING_LITERAL(x) \
		(sizeof(#x)>3&&#x[0]=='u'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
#define _yIO_IS_32_BIT_WIDE_STRING_LITERAL(x) \
		(sizeof(#x)>3&&#x[0]=='U'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
/**
 * Detect if @b x is a wide string literal.
 * @param x a wide string literal like L"abc" or anything else
 * @return nonzero if @b x is a wide string literal, anything else otherwise
 */
#define _yIO_IS_WIDE_STRING_LITERAL(x) \
		(sizeof(#x)>3&&#x[0]=='L'&&#x[1]=='"'&&#x[sizeof(#x)-2]=='"')
/**
 * Detect if @b x is a string literal.
 * @param x Can be a string literal like u8"abc" or L"def" or anything else.
 * @return nonzero if @b x is any kind of string literal, anything else otherwise
 */
#define _yIO_IS_STRING_LITERAL(x)(\
		sizeof(#x)>2&&#x[sizeof(#x)-2]=='"'&&(\
				#x[0]=='"'||(sizeof(#x)>3&&(#x[0]=='u'||#x[0]=='U'||#x[0]=='L')&&#x[1]=='"')\
				||\
				(sizeof(#x)>4&&#x[0]=='u'&&#x[1]=='8'&&#x[2]=='"')\
		)\
)

/**
 * @}
 */

