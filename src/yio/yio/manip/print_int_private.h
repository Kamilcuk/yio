/**
 * @file
 * @date 2026-05-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
// These functions are exported for printing bool, char, void *, wchar_t as integers.
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
#define YYIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"
#include <stdbool.h>
#include <limits.h>

#ifndef YYIO_PRIVATE
#error
#endif

{% call(V) j_FOREACHAPPLY([
    ["uint", "unsigned int", "1"],
    ["ulong", "unsigned long", "1"],
    ["ullong", "unsigned long long", "YYIO_HAS_LLONG"],
    ["uint128", "unsigned __int128", "YYIO_HAS_INT128"]
]) %}
#if $3
int YYIO_print_$1_in(yio_printctx_t *t, $2 arg, bool is_negative);
static inline int YYIO_in_print_$1(yio_printctx_t *t, $2 arg) {
  return YYIO_print_$1_in(t, arg, false);
}
#define YYIO_in_print_$1_generic() \
  ,$2: YYIO_in_print_$1
#endif // $3
{% endcall %}

#define YYIO_in_print_number(t, x) \
  _Generic((x) \
    YYIO_in_print_uint_generic() \
    YYIO_in_print_ulong_generic() \
    YYIO_in_print_ullong_generic() \
    YYIO_in_print_uint128_generic() \
  )(t, x)

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_INT_PRIVATE_H_
