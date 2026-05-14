/**
 * @file
 * @date 2026.05.10
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "print_arr.h"
#include "private.h"
#include <string.h>

static inline int YIO_yio_arr_print(yio_printctx_t *t, const struct YIO_yio_arr_s *arr) {
  const char *const sep = arr->sep != NULL ? arr->sep : ", ";
  const size_t seplen = strlen(sep);
  const char *ptr = (const char *)arr->arr;
  const size_t count = arr->count;
  int err;
  assert(arr->printer);
  for (size_t i = 0; i < count; ++i) {
    if (i) {
      err = yio_printctx_put(t, sep, seplen);
      if (err) { return err; }
    }
    err = arr->printer(t, ptr, arr->elemsize);
    if (err) { return err; }
    ptr += arr->elemsize;
  }
  return 0;
}

int YIO_yio_arr(yio_printctx_t *t) {
  const struct YIO_yio_arr_s *const arr = yio_printctx_va_arg(t, const struct YIO_yio_arr_s *);
  int err = yio_printctx_init(t);
  if (err) { return err; }
  return YIO_yio_arr_print(t, arr);
}
