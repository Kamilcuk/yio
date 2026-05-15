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
#include "portability.h"

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

#ifndef __SDCC
#define YIO_HAS_SYS_TIME_H 1
#else
#define YIO_HAS_SYS_TIME_H 0
#endif

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_CONFIG_H_
