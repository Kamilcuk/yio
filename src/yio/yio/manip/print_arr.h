/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_ARR_H_
#define YYIO_YIO_YIO_MANIP_PRINT_ARR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"
#include <assert.h>
#include <stddef.h>

typedef int(YYIO_arr_printer)(yio_printctx_t *t, const void *pnt, size_t elemsize);

struct YYIO_yio_arr_s {
  const void *arr;
  size_t elemsize;
  size_t count;
  const char *sep;
  YYIO_arr_printer *printer;
};

int YYIO_yio_arr(yio_printctx_t *t);

#ifndef __cplusplus
#define YYIO_PRINT_ARR_DECLARE(T, FUNC) \
  static inline int YYIO_XCONCAT(YYIO_print_arr_, FUNC)( \
      yio_printctx_t * t, const void *pnt, size_t elemsize \
  );
#define YYIO_PRINT_ARR_DECLARE_ALIAS(T, FUNC, SUFFIX) \
  static inline int YYIO_XCONCAT(YYIO_print_arr_, FUNC##SUFFIX)( \
      yio_printctx_t * t, const void *pnt, size_t elemsize \
  );

int YYIO_printctx_print_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, ...);

#define YYIO_PRINT_ARR_DEFINE_IN(T, FUNCNAME, PRINTER) \
  static inline int FUNCNAME(yio_printctx_t *t, const void *pnt, size_t elemsize) { \
    const T *v = (const T *)pnt; \
    assert(sizeof(T) == elemsize); \
    if (!v) return yio_printctx_put(t, "null", 4); \
    const yio_printdata_t funcs[] = {PRINTER, NULL}; \
    return YYIO_printctx_print_in(t, funcs, 0, *v); \
  }

#define YYIO_PRINT_ARR_DEFINE(T, FUNC) \
  YYIO_PRINT_ARR_DEFINE_IN(T, YYIO_XCONCAT(YYIO_print_arr_, FUNC), FUNC)
#define YYIO_PRINT_ARR_DEFINE_ALIAS(T, FUNC, SUFFIX) \
  YYIO_PRINT_ARR_DEFINE_IN(T, YYIO_XCONCAT(YYIO_print_arr_, FUNC##SUFFIX), FUNC)

#define YYIO_PRINT_ARR_DISPATCH(T, FUNC) , T : YYIO_XCONCAT(YYIO_print_arr_, FUNC)
#define YYIO_PRINT_ARR_DISPATCH_ALIAS(T, FUNC, SUFFIX) , T : YYIO_XCONCAT(YYIO_print_arr_, FUNC##SUFFIX)

#define YYIO_PRINT_FUNC_GENERIC_ARR(arg) \
  _Generic((arg)YYIO_PRINT_FUNC_GENERIC_CASES(YYIO_PRINT_ARR_DISPATCH, YYIO_PRINT_ARR_DISPATCH_ALIAS))
#endif

#if (defined(__GNUC__) || defined(__clang__)) && !defined(__cplusplus)
#define YYIO_COUNTOF(A) \
  __extension__({ \
    _Static_assert( \
        !__builtin_types_compatible_p(__typeof__(A), __typeof__(&(A)[0])), \
        "requires a static array. For pointers, use yio_arr(ptr, count)." \
    ); \
    (size_t)sizeof(A) / (size_t)sizeof((A)[0]); \
  })
#define YYIO_COUNTOF_BUT_SEP(A, B) \
  __extension__({ \
    _Static_assert( \
        !_Generic((B), char *: 1, const char *: 1, default: 0) || \
            !__builtin_types_compatible_p(__typeof__(A), __typeof__(&(A)[0])), \
        "requires a static array. For pointers, use yio_arr(ptr, count)." \
    ); \
    (size_t)sizeof(A) / (size_t)sizeof((A)[0]); \
  })
#else
#define YYIO_COUNTOF(A) (sizeof(A) / sizeof((A)[0]))
#define YYIO_COUNTOF_BUT_SEP(A, B) (sizeof(A) / sizeof((A)[0]))
#endif

static inline void YYIO_yio_arr_countof_at_least(const void *pnt, size_t elemsize, size_t count) {
#if defined(__GNUC__) || defined(__clang__)
  const size_t obj_size = __builtin_object_size(pnt, 0);
  if (obj_size != (size_t)-1 && obj_size != 0) { assert(obj_size >= elemsize * count); }
#endif
}

YYIO_nn(1, 2, 6) static inline struct YYIO_yio_arr_s *YYIO_yio_arr_mk(
    struct YYIO_yio_arr_s *obj,
    const void *pnt,
    size_t elemsize,
    size_t count,
    const char *sep,
    YYIO_arr_printer *printer
) {
  YYIO_yio_arr_countof_at_least(pnt, elemsize, count);
  *obj = (struct YYIO_yio_arr_s){
      .arr = pnt, .elemsize = elemsize, .count = count, .sep = sep, .printer = printer
  };
  return obj;
}

YYIO_nn() static inline struct YYIO_yio_arr_s *YYIO_yio_arr_mk_count(
    struct YYIO_yio_arr_s *obj,
    const void *pnt,
    size_t elemsize,
    size_t ignore_count,
    size_t count,
    YYIO_arr_printer *printer
) {
  (void)ignore_count;
  return YYIO_yio_arr_mk(obj, pnt, elemsize, count, 0, printer);
}

#define YYIO_yio_arr_1(obj, A) \
  YYIO_yio_arr_mk( \
      obj, A, sizeof((A)[0]), YYIO_COUNTOF(A), NULL, YYIO_PRINT_FUNC_GENERIC_ARR((A)[0]) \
  )
#define YYIO_yio_arr_2(obj, A, B) \
  _Generic((B), char *: YYIO_yio_arr_mk, const char *: YYIO_yio_arr_mk, default: YYIO_yio_arr_mk_count)( \
      obj, A, sizeof((A)[0]), YYIO_COUNTOF_BUT_SEP(A, B), B, YYIO_PRINT_FUNC_GENERIC_ARR((A)[0]) \
  )
#define YYIO_yio_arr_3(obj, A, C, S) \
  YYIO_yio_arr_mk(obj, A, sizeof((A)[0]), (size_t)(C), S, YYIO_PRINT_FUNC_GENERIC_ARR((A)[0]))
#define YYIO_yio_arr_4(obj, A, C, S, P) YYIO_yio_arr_mk(obj, A, sizeof((A)[0]), (size_t)(C), S, P)

#define YYIO_yio_arr_ARG_N(_1, _2, _3, _4, _5, N, ...) YYIO_yio_arr_##N
#define YYIO_yio_arr_N(...) YYIO_yio_arr_ARG_N(dummy, ##__VA_ARGS__, 5, 4, 3, 2, 1)

/**
 * @def yio_arr
 * @brief Print an array or a pointer range with customizable element
 * formatting and separators.
 */
#define yio_arr(arr, ...) \
  yio_callback( \
      YYIO_yio_arr, YYIO_yio_arr_N(__VA_ARGS__)(&(struct YYIO_yio_arr_s){0}, arr, ##__VA_ARGS__) \
  )

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_ARR_H_
