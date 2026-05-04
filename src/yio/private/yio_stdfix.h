/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_STDFIX_H_
#define YYIO_YIO_PRIVATE_YIO_STDFIX_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#ifndef YYIO_HAS_STDFIX_TYPES
#error "YYIO_HAS_STDFIX_TYPES must be defined"
#endif
#ifndef YYIO_HAS_STDFIX_H
#error "YYIO_HAS_STDFIX_H must be defined"
#endif
#if YYIO_HAS_STDFIX_TYPES
#if YYIO_HAS_STDFIX_H
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
#define YYIO_$1 $1
{% endcall %}{% endmacro %}

{# SUFFIX TYPE MACROFIX MAX MIN IBIT FBIT EPSILON #}

{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#if YYIO_HAS_STDFIX_$3

#define YYIO_C_$3(x)  x##$1
#define YYIO_STDFIX_$3  $2
#define YYIO_IF_$3(...)  __VA_ARGS__

{{j_take_compilers_or_default("$3_MAX", "$4")}}
{{j_take_compilers_or_default("$3_MIN", "$5")}}
{{j_take_compilers_or_default("$3_IBIT", "$6")}}
{{j_take_compilers_or_default("$3_FBIT", "$7")}}
{{j_take_compilers_or_default("$3_EPSILON", "$8")}}

#else

#define YYIO_IF_$3(...)

#endif
{% endcall %}

#endif // YYIO_HAS_STDFIX_TYPES

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_STDFIX_H_
