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
#include "../yio_config.h"
#ifndef YYIO_HAS_STDFIX_TYPES
#error
#endif
#ifndef YYIO_HAS_STDFIX_H
#error
#endif
#if YYIO_HAS_STDFIX_TYPES
#include "private.h"
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
{% set j_STDFIX = [
	["hr",   "signed short _Fract",       "SFRACT",   "0X7FP-7HR",                  "(-0.5HR-0.5HR)",         "0",  "7",  "(0x1P-7HR)",],
	["r",    "signed _Fract",             "FRACT",    "0X7FFFP-15R",                "(-0.5R-0.5R)",           "0",  "15", "(0x1P-15R)",],
	["lr",   "signed long _Fract",        "LFRACT",   "0X7FFFFFFFP-31LR",           "(-0.5LR-0.5LR)",         "0",  "31", "(0x1P-31LR)",],
	["llr",  "signed long long _Fract",   "LLFRACT",  "0X7FFFFFFFFFFFFFFFP-63LLR",  "(-0.5LLR-0.5LLR)",       "0",  "63", "(0x1P-63LLR)",],
	["uhr",  "unsigned short _Fract",     "USFRACT",  "0XFFP-8UHR",                 "0.0UHR",                 "0",  "8",  "(0x1P-8UHR)",],
	["ur",   "unsigned _Fract",           "UFRACT",   "0XFFFFP-16UR",               "0.0UR",                  "0",  "16", "(0x1P-16UR)",],
	["ulr",  "unsigned long _Fract",      "ULFRACT",  "0XFFFFFFFFP-32ULR",          "0.0ULR",                 "0",  "32", "(0x1P-32ULR)",],
	["ullr", "unsigned long long _Fract", "ULLFRACT", "0XFFFFFFFFFFFFFFFFP-64ULLR", "0.0ULLR",                "0",  "64", "(0x1P-64ULLR)",],
	["hk",   "signed short _Accum",       "SACCUM",   "0X7FFFP-7HK",                "(-0X1P7HK-0X1P7HK)",     "8",  "7",  "(0x1P-7HK)",],
	["k",    "signed _Accum",             "ACCUM",    "0X7FFFFFFFP-15K",            "(-0X1P15K-0X1P15K)",     "16", "15", "(0x1P-15K)",],
	["lk",   "signed long _Accum",        "LACCUM",   "0X7FFFFFFFFFFFFFFFP-31LK",   "(-0X1P31LK-0X1P31LK)",   "32", "31", "(0x1P-31LK)",],
	["llk",  "signed long long _Accum",   "LLACCUM",  "0X7FFFFFFFFFFFFFFFP-31LLK",  "(-0X1P31LLK-0X1P31LLK)", "32", "31", "(0x1P-31LLK)",],
	["uhk",  "unsigned short _Accum",     "USACCUM",  "0XFFFFP-8UHK",               "0.0UHK",                 "8",  "8",  "(0x1P-8UHK)",],
	["uk",   "unsigned _Accum",           "UACCUM",   "0XFFFFFFFFP-16UK",           "0.0UK",                  "16", "16", "(0x1P-16UK)",],
	["ulk",  "unsigned long _Accum",      "ULACCUM",  "0XFFFFFFFFFFFFFFFFP-32ULK",  "0.0ULK",                 "32", "32", "(0x1P-32ULK)",],
	["ullk", "unsigned long long _Accum", "ULLACCUM", "0XFFFFFFFFFFFFFFFFP-32ULLK", "0.0ULLK",                "32", "32", "(0x1P-32ULLK)",],
	]
%}

{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef __$3_MAX__

#define YYIO_C_$3(x)  x##$1
#define YYIO_STDFIX_$3  $2

{{j_take_compilers_or_default("$3_MAX", "$4")}}
{{j_take_compilers_or_default("$3_MIN", "$5")}}
{{j_take_compilers_or_default("$3_IBIT", "$6")}}
{{j_take_compilers_or_default("$3_FBIT", "$7")}}
{{j_take_compilers_or_default("$3_EPSILON", "$8")}}

#define YYIO_BITS_$3  ((YYIO_$3_IBIT + YYIO_$3_FBIT + 1)/8*8)

#if   YYIO_BITS_$3 == 8
#define YYIO_UINT_$3  uint_least8_t
#elif YYIO_BITS_$3 == 16
#define YYIO_UINT_$3  uint_least16_t
#elif YYIO_BITS_$3 == 32
#define YYIO_UINT_$3  uint_least32_t
#elif YYIO_BITS_$3 == 64
#define YYIO_UINT_$3  uint_least64_t
#else
#error "I did not expect that one $2!"
#endif

#endif
{% endcall %}

#endif // YYIO_HAS_STDFIX_TYPES
#endif /* YYIO_YIO_PRIVATE_YIO_STDFIX_H_ */
