/**
 * @file
 * @date 05.04.2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef _yIO_YIO_YIO_CONFIG_H_
#define _yIO_YIO_YIO_CONFIG_H_

// {#+
#ifndef _yIO_CDT_PARSER
// +#}
#include "yio_config_gen.h"
// {#+
#else // _yIO_CDT_PARSER

#include <yio/yio_config_gen.h>

#include <wchar.h>
/// The character, changes depending on mode.
#define TCHAR          wchar_t
/// The character constant.
#define TC(x)          L##x
#define TEOF           WEOF
#define TISDIGIT(x)    iswdigit(x)
#define TSTRCHR(x, y)  wcschr(x, y)
#define TSTRCMP(x, y)  wcscmp(x, y)
#define TFPRINTF(...)  fprintf(__VA_ARGS__)
#define TSTRLEN(x)     wcslen(x)
#endif // _yIO_CDT_PARSER
// +#}

#ifndef _yIO_HAS_UNISTD_H
#error _yIO_HAS_UNISTD_H
#endif

// check
#if !_yIO_HAS_UNISTD_H
#if YIO_USE_OUTPUT_FD
#error "You can't output to fd without unistd.h"
#endif
#endif

/**
 * @defgroup compilermacros
 * @brief Macros used for compiler compatibility
 * @{
 */
#ifdef __GNUC__
#define _yIO_nn(...)          __attribute__((__nonnull__(__VA_ARGS__)))
#define _yIO_wur              __attribute__((__warn_unused_result__))
#define _yIO_retmalloc        __attribute__((__malloc__))
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
#ifndef _yIO_retmalloc
/// Function allocates new memory and returns it.
#define _yIO_retmalloc
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

#endif /* _yIO_YIO_YIO_CONFIG_H_ */
