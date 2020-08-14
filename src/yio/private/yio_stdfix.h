/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
#if _yIO_HAS_STDFIX_TYPES
#include "private.h"
#if _yIO_HAS_STDFIX_H
#include <stdfix.h>
#endif

m4_define_function(`m4_take_compilers_or_default~,`m4_dnl;
#ifndef $1
#ifdef __$1__
#define $1  __$1__
#else // __$1__
#define $1  $2
#endif // __$1__
#endif // $1
#define _yIO_$1 $1
~)

m4_dnl SUFFIX TYPE MACROFIX MAX MIN IBIT FBIT EPSILON
m4_define(`m4_stdfix_types~, `m4_dnl;
	(hr,   signed short _Fract,       SFRACT,   0X7FP-7HR,                  (-0.5HR-0.5HR),         0,  7,  0x1P-7HR),
	(r,    signed _Fract,             FRACT,    0X7FFFP-15R,                (-0.5R-0.5R),           0,  15, 0x1P-15R),
	(lr,   signed long _Fract,        LFRACT,   0X7FFFFFFFP-31LR,           (-0.5LR-0.5LR),         0,  31, 0x1P-31LR),
m4_dnl (llr,  signed long long _Fract,   LLFRACT,  0X7FFFFFFFFFFFFFFFP-63LLR,  (-0.5LLR-0.5LLR),       0,  63, 0x1P-63LLR),
	(uhr,  unsigned short _Fract,     USFRACT,  0XFFP-8UHR,                 0.0UHR,                 0,  8,  0x1P-8UHR),
	(ur,   unsigned _Fract,           UFRACT,   0XFFFFP-16UR,               0.0UR,                  0,  16, 0x1P-16UR),
	(ulr,  unsigned long _Fract,      ULFRACT,  0XFFFFFFFFP-32ULR,          0.0ULR,                 0,  32, 0x1P-32ULR),
m4_dnl (ullr, unsigned long long _Fract, ULLFRACT, 0XFFFFFFFFFFFFFFFFP-64ULLR, 0.0ULLR,                0,  64, 0x1P-64ULLR),
	(hk,   signed short _Accum,       SACCUM,   0X7FFFP-7HK,                (-0X1P7HK-0X1P7HK),     8,  7,  0x1P-7HK),
	(k,    signed _Accum,             ACCUM,    0X7FFFFFFFP-15K,            (-0X1P15K-0X1P15K),     16, 15, 0x1P-15K),
	(lk,   signed long _Accum,        LACCUM,   0X7FFFFFFFFFFFFFFFP-31LK,   (-0X1P31LK-0X1P31LK),   32, 31, 0x1P-31LK),
m4_dnl (llk,  signed long long _Accum,   LLACCUM,  0X7FFFFFFFFFFFFFFFP-31LLK,  (-0X1P31LLK-0X1P31LLK), 32, 31, 0x1P-31LLK),
	(uhk,  unsigned short _Accum,     USACCUM,  0XFFFFP-8UHK,               0.0UHK,                 8,  8,  0x1P-8UHK),
	(uk,   unsigned _Accum,           UACCUM,   0XFFFFFFFFP-16UK,           0.0UK,                  16, 16, 0x1P-16UK),
	(ulk,  unsigned long _Accum,      ULACCUM,  0XFFFFFFFFFFFFFFFFP-32ULK,  0.0ULK,                 32, 32, 0x1P-32ULK),
m4_dnl (ullk, unsigned long long _Accum, ULLACCUM, 0XFFFFFFFFFFFFFFFFP-32ULLK, 0.0ULLK,                32, 32, 0x1P-32ULLK),
~) m4_dnl;

m4_applyforeachdefine((m4_stdfix_types), `m4_dnl;

#define _yIO_C_$3(x)  x##$1
#define _yIO_TYPE_$3  $2

m4_take_compilers_or_default($3_MAX, $4)
m4_take_compilers_or_default($3_MIN, $5)
m4_take_compilers_or_default($3_IBIT, $6)
m4_take_compilers_or_default($3_FBIT, $7)
m4_take_compilers_or_default($3_EPSILON, $8)

#define _yIO_BITS_$3  ((_yIO_$3_IBIT + _yIO_$3_FBIT + 1)/8*8)

#if   _yIO_BITS_$3 == 8
#define _yIO_UINT_$3  uint_least8_t
#elif _yIO_BITS_$3 == 16
#define _yIO_UINT_$3  uint_least16_t
#elif _yIO_BITS_$3 == 32
#define _yIO_UINT_$3  uint_least32_t
#elif _yIO_BITS_$3 == 64
#define _yIO_UINT_$3  uint_least64_t
#else
#error "I did not expect that one $2!"
#endif

~)m4_dnl m4_applyforeachdefine ;

#endif // _yIO_HAS_STDFIX_TYPES
