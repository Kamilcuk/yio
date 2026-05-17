/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_ARR_H_
#define YIO_YIO_YIO_MANIP_PRINT_ARR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"
#include <assert.h>
#include <stddef.h>

typedef int(YIO_arr_printer)(yio_printctx_t *t, const void *pnt, size_t elemsize);

struct YIO_yio_arr_s {
  const void *arr;
  size_t elemsize;
  size_t count;
  const char *sep;
  YIO_arr_printer *printer;
};

int YIO_yio_arr(yio_printctx_t *t);

#ifndef __cplusplus
#define YIO_PRINT_ARR_DECLARE(T, FUNC) \
  static inline int YIO_XCONCAT(YIO_print_arr_, FUNC)( \
      yio_printctx_t * t, const void *pnt, size_t elemsize \
  );
#define YIO_PRINT_ARR_DECLARE_ALIAS(T, FUNC, SUFFIX) \
  static inline int YIO_XCONCAT(YIO_print_arr_, FUNC##SUFFIX)( \
      yio_printctx_t * t, const void *pnt, size_t elemsize \
  );

int YIO_printctx_print_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, ...);

#define YIO_PRINT_ARR_DEFINE_IN(T, FUNCNAME, PRINTER) \
  static inline int FUNCNAME(yio_printctx_t *t, const void *pnt, size_t elemsize) { \
    typedef T yio_T; \
    const yio_T *const v = (const yio_T *)pnt; \
    assert(sizeof(T) == elemsize); \
    if (!v) return yio_printctx_put(t, "null", 4); \
    const yio_printdata_t funcs[] = {PRINTER, NULL}; \
    return YIO_printctx_print_in(t, funcs, 0, *v); \
  }

#define YIO_PRINT_ARR_DEFINE(T, FUNC) \
  YIO_PRINT_ARR_DEFINE_IN(T, YIO_XCONCAT(YIO_print_arr_, FUNC), FUNC)
#define YIO_PRINT_ARR_DEFINE_ALIAS(T, FUNC, SUFFIX) \
  YIO_PRINT_ARR_DEFINE_IN(T, YIO_XCONCAT(YIO_print_arr_, FUNC##SUFFIX), FUNC)

#define YIO_PRINT_ARR_DISPATCH(T, FUNC) , T : YIO_XCONCAT(YIO_print_arr_, FUNC)
#define YIO_PRINT_ARR_DISPATCH_ALIAS(T, FUNC, SUFFIX) \
  , T : YIO_XCONCAT(YIO_print_arr_, FUNC##SUFFIX)

#define YIO_PRINT_FUNC_GENERIC_ARR(arg) \
  _Generic((arg)YIO_PRINT_FUNC_GENERIC_CASES(YIO_PRINT_ARR_DISPATCH, YIO_PRINT_ARR_DISPATCH_ALIAS))
#endif

#if (defined(__GNUC__) || defined(__clang__)) && !defined(__cplusplus)
#define YIO_COUNTOF(A) \
  __extension__({ \
    _Static_assert( \
        !__builtin_types_compatible_p(__typeof__(A), __typeof__(&(A)[0])), \
        "requires a static array. For pointers, use yio_arr(ptr, count)." \
    ); \
    (size_t)sizeof(A) / (size_t)sizeof((A)[0]); \
  })
#define YIO_COUNTOF_BUT_SEP(A, B) \
  __extension__({ \
    _Static_assert( \
        !_Generic((B), char *: 1, const char *: 1, default: 0) || \
            !__builtin_types_compatible_p(__typeof__(A), __typeof__(&(A)[0])), \
        "requires a static array. For pointers, use yio_arr(ptr, count)." \
    ); \
    (size_t)sizeof(A) / (size_t)sizeof((A)[0]); \
  })
#else
#define YIO_COUNTOF(A) (sizeof(A) / sizeof((A)[0]))
#define YIO_COUNTOF_BUT_SEP(A, B) (sizeof(A) / sizeof((A)[0]))
#endif

static inline void YIO_yio_arr_countof_at_least(const void *pnt, size_t elemsize, size_t count) {
#if defined(__GNUC__) || defined(__clang__)
  const size_t obj_size = __builtin_object_size(pnt, 0);
  if (obj_size != (size_t)-1 && obj_size != 0) { assert(obj_size >= elemsize * count); }
#endif
}

YIO_nn(1, 2, 6) static inline struct YIO_yio_arr_s *YIO_yio_arr_mk(
    struct YIO_yio_arr_s *obj,
    const void *pnt,
    size_t elemsize,
    size_t count,
    const char *sep,
    YIO_arr_printer *printer
) {
  YIO_yio_arr_countof_at_least(pnt, elemsize, count);
  *obj = (struct YIO_yio_arr_s){
      .arr = pnt, .elemsize = elemsize, .count = count, .sep = sep, .printer = printer
  };
  return obj;
}

YIO_nn() static inline struct YIO_yio_arr_s *YIO_yio_arr_mk_count(
    struct YIO_yio_arr_s *obj,
    const void *pnt,
    size_t elemsize,
    size_t ignore_count,
    size_t count,
    YIO_arr_printer *printer
) {
  (void)ignore_count;
  return YIO_yio_arr_mk(obj, pnt, elemsize, count, 0, printer);
}

#define YIO_yio_arr_1(obj, A) \
  YIO_yio_arr_mk(obj, A, sizeof((A)[0]), YIO_COUNTOF(A), NULL, YIO_PRINT_FUNC_GENERIC_ARR((A)[0]))
#define YIO_yio_arr_2(obj, A, B) \
  _Generic((B), char *: YIO_yio_arr_mk, const char *: YIO_yio_arr_mk, default: YIO_yio_arr_mk_count)( \
      obj, A, sizeof((A)[0]), YIO_COUNTOF_BUT_SEP(A, B), B, YIO_PRINT_FUNC_GENERIC_ARR((A)[0]) \
  )
#define YIO_yio_arr_3(obj, A, C, S) \
  YIO_yio_arr_mk(obj, A, sizeof((A)[0]), (size_t)(C), S, YIO_PRINT_FUNC_GENERIC_ARR((A)[0]))
#define YIO_yio_arr_4(obj, A, C, S, P) YIO_yio_arr_mk(obj, A, sizeof((A)[0]), (size_t)(C), S, P)

#define YIO_yio_arr_ARG_N(_1, _2, _3, _4, _5, N, ...) YIO_yio_arr_##N
#define YIO_yio_arr_N(...) YIO_yio_arr_ARG_N(dummy, ##__VA_ARGS__, 5, 4, 3, 2, 1)

/**
 * @def yio_arr
 * @brief Print an array or a pointer range with customizable element
 * formatting and separators.
 */
#define yio_arr(arr, ...) \
  yio_callback( \
      YIO_yio_arr, YIO_yio_arr_N(__VA_ARGS__)(&(struct YIO_yio_arr_s){0}, arr, ##__VA_ARGS__) \
  )

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_ARR_H_
