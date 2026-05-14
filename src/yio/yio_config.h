/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_CONFIG_H_
#define YIO_YIO_YIO_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif

// {#+
// {#+
#ifndef YIO_CDT_PARSER
// +#} //
#include "yio_config_gen.h"
// {#+
#else // YIO_CDT_PARSER
#include <yio/yio_config_gen.h>
#endif // YIO_CDT_PARSER
// +#} //
// +#} //

#ifndef YIO_HAS_INT128
#ifdef __SIZEOF_INT128__
#define YIO_HAS_INT128 1
#else
#define YIO_HAS_INT128 0
#endif
#endif

#if defined(__BITINT_MAXWIDTH__) && !defined(__SDCC)
#if __BITINT_MAXWIDTH__ < YIO_BITINT_MAXWIDTH
#define YIO_INTERNAL_BITINT_MAXWIDTH __BITINT_MAXWIDTH__
#else
#define YIO_INTERNAL_BITINT_MAXWIDTH YIO_BITINT_MAXWIDTH
#endif
#else
#define YIO_INTERNAL_BITINT_MAXWIDTH 0
#endif

#ifndef YIO_HAS_UNISTRING
#error YIO_HAS_UNISTRING not defined
#endif

#ifndef YIO_HAS_INT128
#error YIO_HAS_INT128 not defined
#endif

#ifndef YIO_HAS_LLONG
#error YIO_HAS_LLONG not defined
#endif

#ifndef YIO_HAS_UNISTD_H
#error YIO_HAS_UNISTD_H not defined
#endif

#ifndef YIO_HAS_FILE
#error YIO_HAS_FILE is not defined
#endif

#include "ptrcheck.h"

/* Backend selection logic -------------------------------------------------- */

#define YIO_BACKEND_ID_INTERNAL_3(prefix, name)  prefix##name
#define YIO_BACKEND_ID_INTERNAL_2(prefix, name)  YIO_BACKEND_ID_INTERNAL_3(prefix, name)
#define YIO_BACKEND_ID(prefix, name)             YIO_BACKEND_ID_INTERNAL_2(prefix, name)

/* Output Backend Selection */
#define YIO_ID_OUT_STDOUT   1
#define YIO_ID_OUT_FD       2
#define YIO_ID_OUT_PUTCHAR  3

#define YIO_OUTPUT_BACKEND_ID  YIO_BACKEND_ID(YIO_ID_OUT_, YIO_OUTPUT_BACKEND)

#if YIO_OUTPUT_BACKEND_ID < 1 || YIO_OUTPUT_BACKEND_ID > 3
#error "Invalid YIO_OUTPUT_BACKEND configuration"
#endif

#define YIO_OUTPUT_BACKEND_STDOUT   (YIO_OUTPUT_BACKEND_ID == YIO_ID_OUT_STDOUT)
#define YIO_OUTPUT_BACKEND_FD       (YIO_OUTPUT_BACKEND_ID == YIO_ID_OUT_FD)
#define YIO_OUTPUT_BACKEND_PUTCHAR  (YIO_OUTPUT_BACKEND_ID == YIO_ID_OUT_PUTCHAR)

/* Feature checks ----------------------------------------------------------- */

#if YIO_OUTPUT_BACKEND_FD && !YIO_HAS_UNISTD_H
#error "You can't use FD output backend without unistd.h"
#endif

#if YIO_OUTPUT_BACKEND_STDOUT && !YIO_HAS_FILE
#error "You can't use STDOUT output backend without FILE support"
#endif

/**
 * @defgroup compilermacros
 * @brief Macros used for compiler compatibility
 * @{
 */
#ifdef __GNUC__
#define YIO_nn(...)          __attribute__((__nonnull__(__VA_ARGS__)))
#define YIO_wur              __attribute__((__warn_unused_result__))
#define YIO_retmalloc        __attribute__((__malloc__))
#define YIO_const            __attribute__((__const__))
#ifndef __INTEL_COMPILER
#define YIO_rnn              __attribute__((__returns_nonnull__))
#endif
#define YIO_format(...)      __attribute__((__format__(__VA_ARGS__)))
#define YIO_deprecated(...)  __attribute__((__deprecated__(__VA_ARGS__)))
#define YIO_warning(...)     __attribute__((__warning__(__VA_ARGS__)))
#if !__clang__
#define YIO_error(...)       __attribute__((__error__(__VA_ARGS__)))
#endif
#if __GNUC__ >= 10
#define YIO_access_r(...)    __attribute__((__access__(__read_only__, ##__VA_ARGS__)))
#define YIO_access_w(...)    __attribute__((__access__(__write_only__, ##__VA_ARGS__)))
#define YIO_access_rw(...)   __attribute__((__access__(__read_write__, ##__VA_ARGS__)))
#endif
#endif // __GNUC__

#ifndef YIO_nn
/// Mark that function returns nonnull.
#define YIO_nn(...)
#endif
#ifndef YIO_wur
/// Warn on function unused return value.
#define YIO_wur
#endif
#ifndef YIO_retmalloc
/// Function allocates new memory and returns it.
#define YIO_retmalloc
#endif
#ifndef YIO_const
/// Function is a const-like function.
#define YIO_const
#endif
#ifndef YIO_rnn
/// Function never returns null.
#define YIO_rnn
#endif
#ifndef YIO_format
/// Function formats like printf
#define YIO_format(...)
#endif
#ifndef YIO_deprecated
/// Function is deprecated
#define YIO_deprecated(...)
#endif
#ifndef YIO_warning
/// Function is deprecated
#define YIO_warning(...)
#endif
#ifndef YIO_error
/// Function errors upon calling
#define YIO_error(...)
#endif
#ifndef YIO_access_r
#define YIO_access_r(...)
#endif
#ifndef YIO_access_w
#define YIO_access_w(...)
#endif
#ifndef YIO_access_rw
#define YIO_access_rw(...)
#endif

#define YIO_CONCAT(a, b)   a##b
#define YIO_XCONCAT(a, b)  YIO_CONCAT(a, b)

/**
 * @def YIO_ERROR(ENUM, DESC)
 * @param ENUM The suffix to YIO_ERROR_* enum name.
 * @param DESC The description of the errors
 * @short Is used to automatically parse and register an error code with description.
 */
#define YIO_ERROR(ENUM, DESC)  ENUM

#define YIO_IF_1(...)      __VA_ARGS__
#define YIO_IF_0(...)
#define YIO_IF(x, ...)     YIO_XCONCAT(YIO_IF_, x)(__VA_ARGS__)

#ifdef __SDCC
#define YIO_AT(addr)  __at(addr)
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
#define YIO_REENTRANT __reentrant
#define YIO_XDATA     __xdata
#define YIO_CODE      __code
#else
#define YIO_REENTRANT
#define YIO_XDATA
#define YIO_CODE
#endif
#define YIO_HAS_SYS_TIME_H 0
#else
#define YIO_REENTRANT
#define YIO_XDATA
#define YIO_CODE
#define YIO_AT(addr)
#define YIO_HAS_SYS_TIME_H 1
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_CONFIG_H_
