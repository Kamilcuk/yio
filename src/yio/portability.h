/**
 * @file
 * @brief Compiler and platform portability macros.
 */
#ifndef YIO_YIO_PORTABILITY_H_
#define YIO_YIO_PORTABILITY_H_
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__has_attribute)
#if __has_attribute(yio_format)
#define YIO_FORMAT(N) __attribute__((yio_format(N)))
#endif
#endif
#ifndef YIO_FORMAT
#define YIO_FORMAT(N)
#endif

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
#define YIO_nn(...)
#endif
#ifndef YIO_wur
#define YIO_wur
#endif
#ifndef YIO_retmalloc
#define YIO_retmalloc
#endif
#ifndef YIO_const
#define YIO_const
#endif
#ifndef YIO_rnn
#define YIO_rnn
#endif
#ifndef YIO_format
#define YIO_format(...)
#endif
#ifndef YIO_deprecated
#define YIO_deprecated(...)
#endif
#ifndef YIO_warning
#define YIO_warning(...)
#endif
#ifndef YIO_error
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
#else
#define YIO_REENTRANT
#define YIO_XDATA
#define YIO_CODE
#define YIO_AT(addr)
#endif

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PORTABILITY_H_
