/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_STDFIX_H_
#define YIO_YIO_PRIVATE_YIO_STDFIX_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#ifndef YIO_HAS_STDFIX_TYPES
#error "YIO_HAS_STDFIX_TYPES must be defined"
#endif
#ifndef YIO_HAS_STDFIX_H
#error "YIO_HAS_STDFIX_H must be defined"
#endif
#if YIO_HAS_STDFIX_TYPES
#if YIO_HAS_STDFIX_H
#include <stdfix.h>
#endif

{% macro j_take_compilers_or_default(A, B) %}{% call j_APPLY(A, B) %}
#ifndef $1
# ifdef __$1__
#  define $1 __$1__
# else
#  define $1 $2
# endif
#endif
#define YIO_$1 $1
{% endcall %}{% endmacro %}

{# SUFFIX TYPE MACROFIX MAX MIN IBIT FBIT EPSILON #}

{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#if YIO_HAS_STDFIX_$3

#define YIO_C_$3(x)  x##$1
#define YIO_STDFIX_$3  $2
#define YIO_IF_$3(...)  __VA_ARGS__

{{j_take_compilers_or_default("$3_MAX", "$4")}}
{{j_take_compilers_or_default("$3_MIN", "$5")}}
{{j_take_compilers_or_default("$3_IBIT", "$6")}}
{{j_take_compilers_or_default("$3_FBIT", "$7")}}
{{j_take_compilers_or_default("$3_EPSILON", "$8")}}

#else

#define YIO_IF_$3(...)

#endif
{% endcall %}

#endif // YIO_HAS_STDFIX_TYPES

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_STDFIX_H_
