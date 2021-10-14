/**
 * @file
 * @date 21 kwi 2020
 * @author Kamil Cukrowski
 * @copyright SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

enum _yIO_TYPES {
	_yIO_TYPE_YIO  = 0,
	_yIO_TYPE_YWIO = 0,
	_yIO_TYPE_YUIO = 0,
};

#if YIO_REALLY_CDT_PARSER

// «
#define m4_regexp(...)
#define m4_ifelse(...)
#define m4_patsubst(...)
// My functions:
#define m4_syncline(...)
#define m4_syncline_dnl(...)
#define m4_template_chooser(...)
#define m4_template_chooser2(...)
#define m4_applyforeachdefine(...)
// »

#define _yIO_TYPE_YIO 1

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

#endif

/**
 * @defgroup compilermacros
 * @brief Macros used for compiler compatibility
 * @{
 */
#ifdef __GNUC__
#define _yIO_nn(...)          __attribute__((__nonnull__(__VA_ARGS__)))
#define _yIO_wur              __attribute__((__warn_unused_result__))
#define _yIO_malloc           __attribute__((__malloc__))
#define _yIO_const            __attribute__((__const__))
#ifndef __INTEL_COMPILER
#define _yIO_rnn              __attribute__((__returns_nonnull__))
#endif
#define _yIO_format(...)      __attribute__((__format__(__VA_ARGS__)))
#define _yIO_deprecated(...)  __attribute__((__deprecated__(__VA_ARGS__)))
#define _yIO_warning(...)     __attribute__((__warning__(__VA_ARGS__)))
#if !__clang__
#define _yIO_error(...)       __attribute__((__error__(__VA_ARGS__)))
#endif
#if __GNUC__ >= 10
#define _yIO_access_r(...)    __attribute__((__access__(__read_only__, ##__VA_ARGS__)))
#define _yIO_access_w(...)    __attribute__((__access__(__write_only__, ##__VA_ARGS__)))
#define _yIO_access_rw(...)   __attribute__((__access__(__read_write__, ##__VA_ARGS__)))
#endif
#endif // __GNUC__

#ifndef _yIO_nn
/// Mark that function returns nonnull.
#define _yIO_nn(...)
#endif
#ifndef _yIO_wur
/// Warn on function unused return value.
#define _yIO_wur
#endif
#ifndef _yIO_malloc
/// Function allocates new memory and returns it.
#define _yIO_malloc
#endif
#ifndef _yIO_const
/// Function is a const-like function.
#define _yIO_const
#endif
#ifndef _yIO_rnn
/// Function never returns null.
#define _yIO_rnn
#endif
#ifndef _yIO_format
/// Function formats like printf
#define _yIO_format(...)
#endif
#ifndef _yIO_deprecated
/// Function is deprecated
#define _yIO_deprecated(...)
#endif
#ifndef _yIO_warning
/// Function is deprecated
#define _yIO_warning(...)
#endif
#ifndef _yIO_error
/// Function errors upon calling
#define _yIO_error(...)
#endif
#ifndef _yIO_access_r
#define _yIO_access_r(...)
#endif
#ifndef _yIO_access_w
#define _yIO_access_w(...)
#endif
#ifndef _yIO_access_rw
#define _yIO_access_rw(...)
#endif

/**
 * @}
 */
