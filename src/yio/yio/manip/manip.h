/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_MANIP_MANIP_H_
#define YIO_YIO_YIO_MANIP_MANIP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "print_bool.h"
#include "print_complex.h"
#include "print_count.h"
#include "print_float.h"
#include "print_int.h"
#include "print_mon.h"
#include "print_pfmt.h"
#include "print_repr.h"
#include "print_stdfix.h"
#include "print_timevalspec.h"
#include "print_tm.h"
#include "print_wchars.h"

/**
 * @def YIO_COUNTER
 * Internal preprocessor counter for adding types into _Generic.
 * Incremented by including YIO_ADD_TYPE_INC file.
 */
#define YIO_COUNTER 0

/**
 * @def YIO_PRINT_FUNC_GENERIC_SLOTS
 * Callback for handling user _Generic types
 */
#define YIO_PRINT_FUNC_GENERIC_SLOTS(X, XALIAS) /**/

#ifndef YIO_HAS_UNIQUE_CONSTPOINTER
#error YIO_HAS_UNIQUE_CONSTPOINTER is not defined
#endif

#ifdef __cplusplus
#define YIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC) \
  inline yio_printdata_t yyio_print_func_generic_cpp(TYPE) { return (yio_printdata_t)(FUNC); }
#define YIO_OVERLOAD_TYPE_FUNC_ALIAS(TYPE, FUNC, SUFFIX) YIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC)
#define YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, BASETYPE, FUNC) \
  X(BASETYPE *, FUNC) \
  XALIAS(const BASETYPE *, FUNC, _constptr)
#else // __cplusplus
#define YIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC) , TYPE : FUNC
#define YIO_OVERLOAD_TYPE_FUNC_ALIAS(TYPE, FUNC, SUFFIX) YIO_OVERLOAD_TYPE_FUNC(TYPE, FUNC)
#if defined(__SDCC_mcs51) || defined(__SDCC_ds390) || defined(__SDCC_ds400)
#define YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, BASETYPE, FUNC) \
  X(BASETYPE __xdata *, FUNC) \
  XALIAS(BASETYPE __code *, FUNC, _code) \
  XALIAS(BASETYPE __pdata *, FUNC, _pdata) \
  XALIAS(BASETYPE __idata *, FUNC, _idata) \
  XALIAS(BASETYPE __data *, FUNC, _data)
#elif defined(__SDCC_stm8)
#define YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, BASETYPE, FUNC) X(BASETYPE *, FUNC)
#else // __SDCC
#define YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, BASETYPE, FUNC) \
  X(BASETYPE *, FUNC) \
  YIO_IF(YIO_HAS_UNIQUE_CONSTPOINTER, XALIAS(const BASETYPE *, FUNC, _constptr))
#endif // __SDCC
#endif // __cplusplus

// print_wchars.c
#ifndef YIO_HAS_UCHAR_H
#error YIO_HAS_UCHAR_H is not defined
#endif
#if YIO_HAS_UCHAR_H
#include <uchar.h>
int YIO_print_constchar16pnt(yio_printctx_t *t);
int YIO_print_constchar32pnt(yio_printctx_t *t);
#define YIO_PRINT_FUNC_GENERIC_UCHARS(X, XALIAS) \
  YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, char16_t, YIO_print_constchar16pnt) \
  YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, char32_t, YIO_print_constchar32pnt)
#else
#define YIO_PRINT_FUNC_GENERIC_UCHARS(X, XALIAS)
#endif

/**
 * 
 * @def YIO_PRINT_FUNC_GENERIC_CASES
 * @param X Callback for main types. Takes (TYPE, FUNCTION).
 * @param XALIAS Callback for alias types. Takes (TYPE, FUNCTION, SUFFIX).
 */
#define YIO_PRINT_FUNC_GENERIC_CASES(X, XALIAS) \
  X(bool, YIO_print_bool) \
  X(char, YIO_print_char) \
  YIO_PRINT_FUNC_GENERIC_SLOTS(X, XALIAS) \
  YIO_PRINT_SCHAR(X, XALIAS) \
  YIO_PRINT_UCHAR(X, XALIAS) \
  YIO_PRINT_INTS(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_INTS_INT128(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_BITINTS(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_WCHARS(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_UCHARS(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_FLOATS(X, XALIAS) \
  YIO_PRINT_GENERIC_TIMESPEC(X, XALIAS) \
  YIO_PRINT_GENERIC_TIMEVAL(X, XALIAS) \
  YIO_PRINT_GENERIC_TM(X, XALIAS) \
  YIO_PRINT_STDFIX(X, XALIAS) \
  YIO_PRINT_COMPLEX(X, XALIAS) \
  YIO_PRINT_FUNC_GENERIC_WCHARS_SECOND_STAGE(X, XALIAS) \
  YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, char, YIO_print_constcharpnt) \
  YIO_OVERLOAD_POINTER_TYPE_FUNC(X, XALIAS, void, YIO_print_voidp)

#ifdef __cplusplus
extern "C++" {
namespace yyio_cpp {
YIO_PRINT_FUNC_GENERIC_CASES(YIO_OVERLOAD_TYPE_FUNC, YIO_OVERLOAD_TYPE_FUNC_ALIAS)
}
}
#endif

#ifdef __cplusplus
#define YIO_PRINT_FUNC_GENERIC(arg, ...) yyio_cpp::yyio_print_func_generic_cpp(arg)
#elif defined(__SDCC)
// In sdcc _Generic("string", char[7]:1) matches.
// To decay a string into a pointer, ternary expression can be used.
#define YIO_PRINT_FUNC_GENERIC(arg, ...) \
  _Generic(1 ? (arg) : (arg)YIO_PRINT_FUNC_GENERIC_CASES(YIO_OVERLOAD_TYPE_FUNC, YIO_OVERLOAD_TYPE_FUNC_ALIAS))
#else
#define YIO_PRINT_FUNC_GENERIC(arg, ...) \
  _Generic((arg)YIO_PRINT_FUNC_GENERIC_CASES(YIO_OVERLOAD_TYPE_FUNC, YIO_OVERLOAD_TYPE_FUNC_ALIAS))
#endif

// clang-format off
/**
 * @def YIO_ADD_TYPE_INC
 * Include this file after using YIO_ADD_TYPE macro to add addional type to handle.
 */
#define YIO_ADD_TYPE_INC() <yio/yio/manip/slots.h> // NOLINT
// clang-format on

/**
 * @def YIO_ADD_TYPE
 * @see YIO_ADD_TYPE_INC
 */
#ifdef __cplusplus
#define YIO_ADD_TYPE(TYPE, FUNCTION) \
  typedef TYPE YIO_XCONCAT(YIO_TYPE_, YIO_COUNTER); \
  static inline int YIO_XCONCAT(YIO_TYPE_FUNC_, YIO_COUNTER)(yio_printctx_t * ctx) { \
    return (FUNCTION)(ctx); \
  } \
  namespace yyio_cpp { \
  YIO_OVERLOAD_TYPE_FUNC(TYPE, FUNCTION) \
  }
#elif defined(__GNUC__)
#define YIO_ADD_TYPE(TYPE, FUNCTION) \
  typedef typeof(TYPE) YIO_XCONCAT(YIO_TYPE_, YIO_COUNTER); \
  static inline int YIO_XCONCAT(YIO_TYPE_FUNC_, YIO_COUNTER)(yio_printctx_t * ctx) \
      __attribute__((__alias__(#FUNCTION)));
#else
#define YIO_ADD_TYPE(TYPE, FUNCTION) \
  typedef TYPE YIO_XCONCAT(YIO_TYPE_, YIO_COUNTER); \
  static inline int YIO_XCONCAT(YIO_TYPE_FUNC_, YIO_COUNTER)(yio_printctx_t * ctx) { \
    return (FUNCTION)(ctx); \
  }
#endif

#include "print_arr.h"
#ifndef __cplusplus
YIO_PRINT_FUNC_GENERIC_CASES(YIO_PRINT_ARR_DECLARE, YIO_PRINT_ARR_DECLARE_ALIAS)
YIO_PRINT_FUNC_GENERIC_CASES(YIO_PRINT_ARR_DEFINE, YIO_PRINT_ARR_DEFINE_ALIAS)
#endif

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_MANIP_H_
