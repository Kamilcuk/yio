/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_CONFIG_H_
#define YYIO_YIO_YIO_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif

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

#ifndef YYIO_HAS_INT128
#ifdef __SIZEOF_INT128__
#define YYIO_HAS_INT128 1
#else
#define YYIO_HAS_INT128 0
#endif
#endif

#if defined(__BITINT_MAXWIDTH__) && !defined(__SDCC)
#if __BITINT_MAXWIDTH__ < YIO_BITINT_MAXWIDTH
#define YYIO_BITINT_MAXWIDTH __BITINT_MAXWIDTH__
#else
#define YYIO_BITINT_MAXWIDTH YIO_BITINT_MAXWIDTH
#endif
#else
#define YYIO_BITINT_MAXWIDTH 0
#endif

#ifndef YYIO_HAS_UNISTRING
#error YYIO_HAS_UNISTRING not defined
#endif

#ifndef YYIO_HAS_INT128
#error YYIO_HAS_INT128 not defined
#endif

#ifndef YYIO_HAS_LLONG
#error YYIO_HAS_LLONG not defined
#endif

#ifndef YYIO_HAS_UNISTD_H
#error YYIO_HAS_UNISTD_H not defined
#endif

#ifndef YYIO_HAS_FILE
#error YYIO_HAS_FILE is not defined
#endif

#include "ptrcheck.h"

/* Backend selection logic -------------------------------------------------- */

#define YYIO_BACKEND_ID_INTERNAL_3(prefix, name)  prefix##name
#define YYIO_BACKEND_ID_INTERNAL_2(prefix, name)  YYIO_BACKEND_ID_INTERNAL_3(prefix, name)
#define YYIO_BACKEND_ID(prefix, name)             YYIO_BACKEND_ID_INTERNAL_2(prefix, name)

/* Output Backend Selection */
#define YYIO_ID_OUT_STDOUT   1
#define YYIO_ID_OUT_FD       2
#define YYIO_ID_OUT_PUTCHAR  3

#define YYIO_OUTPUT_BACKEND_ID  YYIO_BACKEND_ID(YYIO_ID_OUT_, YIO_OUTPUT_BACKEND)

#if YYIO_OUTPUT_BACKEND_ID < 1 || YYIO_OUTPUT_BACKEND_ID > 3
#error "Invalid YIO_OUTPUT_BACKEND configuration"
#endif

#define YIO_OUTPUT_BACKEND_STDOUT   (YYIO_OUTPUT_BACKEND_ID == YYIO_ID_OUT_STDOUT)
#define YIO_OUTPUT_BACKEND_FD       (YYIO_OUTPUT_BACKEND_ID == YYIO_ID_OUT_FD)
#define YIO_OUTPUT_BACKEND_PUTCHAR  (YYIO_OUTPUT_BACKEND_ID == YYIO_ID_OUT_PUTCHAR)

/* Feature checks ----------------------------------------------------------- */

#if YIO_OUTPUT_BACKEND_FD && !YYIO_HAS_UNISTD_H
#error "You can't use FD output backend without unistd.h"
#endif

#if YIO_OUTPUT_BACKEND_STDOUT && !YYIO_HAS_FILE
#error "You can't use STDOUT output backend without FILE support"
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

/**
 * @def YYIO_ERROR(ENUM, DESC)
 * @param ENUM The suffix to YIO_ERROR_* enum name.
 * @param DESC The description of the errors
 * @short Is used to automatically parse and register an error code with description.
 */
#define YYIO_ERROR(ENUM, DESC)  ENUM

#define YYIO_IF_1(...)      __VA_ARGS__
#define YYIO_IF_0(...)
#define YYIO_IF(x, ...)     YYIO_XCONCAT(YYIO_IF_, x)(__VA_ARGS__)

#ifdef __SDCC
#define YYIO_AT(addr)  __at(addr)
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
#define YYIO_REENTRANT __reentrant
#define YYIO_XDATA     __xdata
#define YYIO_CODE      __code
#else
#define YYIO_REENTRANT
#define YYIO_XDATA
#define YYIO_CODE
#endif
#define YYIO_HAS_SYS_TIME_H 0
#else
#define YYIO_REENTRANT
#define YYIO_XDATA
#define YYIO_CODE
#define YYIO_AT(addr)
#define YYIO_HAS_SYS_TIME_H 1
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_CONFIG_H_
