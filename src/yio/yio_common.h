/**
 * @file
 * @date 21 kwi 2020
 * @author Kamil Cukrowski
 * @copyright SPDX-License-Identifier: GPL-3.0-only
 */
m4_config() m4_dnl;
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

enum _yIO_TYPES {
	YIO_TYPE_YIO,
	YIO_TYPE_YWIO,
	YIO_TYPE_YUIO,
};

#if defined __CDT_PARSER__ || YIO_REALLY_CDT_PARSER

#define _Generic(...)  /**/

/**
 * @defgroup privatemacros
 * @brief Private macros for internal use on m4 preprocessing stage
 * These symbols should be defined by m4/cfg_*.m4 files.
 * @{
 */

/**
 * Represents a character. A pointer to it represents a string.
 */
typedef char Ychar;
/**
 * A character or EOF.
 */
typedef int Yint;
/**
 * The EOF character.
 */
#define YEOF EOF
/**
 * @param x A character within single qoutes
 *          or string within double quotes.
 * @brief Defines a character constant.
 */
#define Yc(c)  c
/**
 * @param str <tt>const Ychar*</tt> A zero terminated string.
 * @return The count of characters (not bytes) in a string.
 */
#define Ystrlen(str)      strlen(str)
#define Yfgetc(f)         fgetc(f)
#define Ystrchr(str, c)   strchr(str, c)
#define Yidigit(c)        isdigit((unsigned char)c)
/**
 * @}
 */

#endif // __CDT_PARSER__

/**
 * @defgroup compilermacros
 * @brief Macros used for compiler compatibility
 * @{
 */
#if defined(__GNUC__)
#define _yIO_nn(...)      __attribute__((__nonnull__(__VA_ARGS__)))
#define _yIO_wur          __attribute__((__warn_unused_result__))
#define _yIO_malloc       __attribute__((__malloc__))
#define _yIO_const        __attribute__((__const__))
#define _yIO_rnn          __attribute__((__returns_nonnull__))
#define _yIO_format(...)  __attribute__((__format__(__VA_ARGS__)))
#else
/// Mark that function returns nonnull.
#define _yIO_nn(...)
/// Warn on function unused return value.
#define _yIO_wur
/// Function allocates new memory and returns it.
#define _yIO_malloc
/// Function is a const-like function.
#define _yIO_const
/// Function never returns null.
#define _yIO_rnn
/// Function formats like printf
#define _yIO_format(...)
#endif
#if __GNUC__ > 10
#define _yIO_access(...)  __attribute__((__acces__(__VA_ARGS__)))
#else
/// Attribute access
#define _yIO_access(...)
#endif
/**
 * @}
 */
