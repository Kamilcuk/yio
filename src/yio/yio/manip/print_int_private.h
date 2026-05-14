/**
 * @file
 * @date 2026-05-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
// These functions are exported for printing bool, char, void *, wchar_t as integers.
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
#define YIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"
#include <stdbool.h>
#include <limits.h>

#ifndef YIO_PRIVATE
#error
#endif

{% call(V) j_FOREACHAPPLY([
    ["uint", "unsigned int", "1"],
    ["ulong", "unsigned long", "1"],
    ["ullong", "unsigned long long", "YIO_HAS_LLONG"],
    ["uint128", "unsigned __int128", "YIO_HAS_INT128"]
]) %}
#if $3
int YIO_print_$1_in(yio_printctx_t *t, $2 arg, bool is_negative);
static inline int YIO_in_print_$1(yio_printctx_t *t, $2 arg) {
  return YIO_print_$1_in(t, arg, false);
}
#define YIO_in_print_$1_generic() \
  ,$2: YIO_in_print_$1
#endif // $3
{% endcall %}

#define YIO_in_print_number(t, x) \
  _Generic((x) \
    YIO_in_print_uint_generic() \
    YIO_in_print_ulong_generic() \
    YIO_in_print_ullong_generic() \
    YIO_in_print_uint128_generic() \
  )(t, x)

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
