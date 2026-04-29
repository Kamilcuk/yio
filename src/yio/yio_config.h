/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_CONFIG_H_
#define YYIO_YIO_YIO_CONFIG_H_

// {#+
// {#+
#ifndef YYIO_CDT_PARSER
// +#} //
#include "yio_config_gen.h"
// {#+
#else // YYIO_CDT_PARSER
#include <yio/yio_config_gen.h>
#endif // YYIO_CDT_PARSER
// +#} //
// +#} //

#ifdef __SIZEOF_INT128__
#define YYIO_HAS_INT128 1
#else
#define YYIO_HAS_INT128 0
#endif

#ifdef __BITINT_MAXWIDTH__
#if __BITINT_MAXWIDTH__ < YIO_BITINT_MAXWIDTH
#define YYIO_BITINT_MAXWIDTH __BITINT_MAXWIDTH__
#else
#define YYIO_BITINT_MAXWIDTH YIO_BITINT_MAXWIDTH
#endif
#else
#define YYIO_BITINT_MAXWIDTH 0
#endif

#ifndef YYIO_HAS_UNISTD_H
#error YYIO_HAS_UNISTD_H
#endif

// check
#if !YYIO_HAS_UNISTD_H
#if YIO_USE_OUTPUT_FD
#error "You can't output to fd without unistd.h"
#endif
#endif

#ifdef __SDCC
#define YYIO_REENTRANT __reentrant
#else
#define YYIO_REENTRANT
#endif

/* If the environment does not provide FILE, we provide stubs to allow compilation. */
#if YYIO_HAS_FILE == 0
#include <errno.h>
#ifndef ENOSYS
#define ENOSYS 38
#endif
#ifndef FILE
typedef struct YYIO_SDCC_FILE_s { int YYIO_SDCC_FILE_unused; } FILE;
#define FILE FILE
#endif
#ifndef stderr
#define stderr ((FILE*)0)
#endif
#ifndef stdout
#define stdout ((FILE*)0)
#endif
#ifndef fprintf
#define fprintf(stream, ...) ((void)(stream), errno = ENOSYS, -1)
#endif
#ifndef fwrite
#define fwrite(ptr, size, nmemb, stream)  \
  ((void)(ptr), (void)(size), (void)(nmemb), (void)(stream), errno = ENOSYS, (size_t)-1)
#endif
#endif

/**
 * @defgroup compilermacros
 * @brief Macros used for compiler compatibility
 * @{
 */
#ifdef __GNUC__
#define YYIO_nn(...)          __attribute__((__nonnull__(__VA_ARGS__)))
#define YYIO_wur              __attribute__((__warn_unused_result__))
#define YYIO_retmalloc        __attribute__((__malloc__))
#define YYIO_const            __attribute__((__const__))
#ifndef __INTEL_COMPILER
#define YYIO_rnn              __attribute__((__returns_nonnull__))
#endif
#define YYIO_format(...)      __attribute__((__format__(__VA_ARGS__)))
#define YYIO_deprecated(...)  __attribute__((__deprecated__(__VA_ARGS__)))
#define YYIO_warning(...)     __attribute__((__warning__(__VA_ARGS__)))
#if !__clang__
#define YYIO_error(...)       __attribute__((__error__(__VA_ARGS__)))
#endif
#if __GNUC__ >= 10
#define YYIO_access_r(...)    __attribute__((__access__(__read_only__, ##__VA_ARGS__)))
#define YYIO_access_w(...)    __attribute__((__access__(__write_only__, ##__VA_ARGS__)))
#define YYIO_access_rw(...)   __attribute__((__access__(__read_write__, ##__VA_ARGS__)))
#endif
#endif // __GNUC__

#ifndef YYIO_nn
/// Mark that function returns nonnull.
#define YYIO_nn(...)
#endif
#ifndef YYIO_wur
/// Warn on function unused return value.
#define YYIO_wur
#endif
#ifndef YYIO_retmalloc
/// Function allocates new memory and returns it.
#define YYIO_retmalloc
#endif
#ifndef YYIO_const
/// Function is a const-like function.
#define YYIO_const
#endif
#ifndef YYIO_rnn
/// Function never returns null.
#define YYIO_rnn
#endif
#ifndef YYIO_format
/// Function formats like printf
#define YYIO_format(...)
#endif
#ifndef YYIO_deprecated
/// Function is deprecated
#define YYIO_deprecated(...)
#endif
#ifndef YYIO_warning
/// Function is deprecated
#define YYIO_warning(...)
#endif
#ifndef YYIO_error
/// Function errors upon calling
#define YYIO_error(...)
#endif
#ifndef YYIO_access_r
#define YYIO_access_r(...)
#endif
#ifndef YYIO_access_w
#define YYIO_access_w(...)
#endif
#ifndef YYIO_access_rw
#define YYIO_access_rw(...)
#endif

#define YYIO_CONCAT(a, b)   a##b
#define YYIO_XCONCAT(a, b)  YYIO_CONCAT(a, b)

#define YYIO_CONCAT3(a, b, c)   a##b##c
#define YYIO_XCONCAT3(a, b, c)  YYIO_CONCAT3(a, b, c)

/**
 * @}
 */

#endif /* YYIO_YIO_YIO_CONFIG_H_ */
