/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once
m4_config() m4_dnl;
#include <yio/yio_config.h>
#if _yIO_HAS_STDFIX
#include <stdfix.h>

#else // _yIO_HAS_STDFIX

m4_define_function(`m4_take_compilers_or_default~,`m4_dnl;
#if !defined($1) && defined(__$1__)
#define $1  __$1__
#else
#define $1  $2
#endif
~)

m4_applyforeachdefine(`(
m4_dnl TYPE     MAX                          MIN                      IBIT  FBIT  EPSILON
	(UFRACT,    0XFFFFP-16UR,                0.0UR,                   0,    16,   0x1P-16UR),
	(USFRACT,   0XFFP-8UHR,                  0.0UHR,                  0,    8,    0x1P-8UHR),
	(ULFRACT,   0XFFFFFFFFP-32ULR,           0.0ULR,                  0,    32,   0x1P-32ULR),
	(ULLFRACT,  0XFFFFFFFFFFFFFFFFP-64ULLR,  0.0ULLR,                 0,    64,   0x1P-64ULLR),

	(SFRACT,    0X7FP-7HR,                   (-0.5HR-0.5HR),          0,    7,    0x1P-7HR),
	(FRACT,     0X7FFFP-15R,                 (-0.5R-0.5R),            0,    15,   0x1P-15R),
	(LFRACT,    0X7FFFFFFFP-31LR,            (-0.5LR-0.5LR),          0,    31,   0x1P-31LR),
	(LLFRACT,   0X7FFFFFFFFFFFFFFFP-63LLR,   (-0.5LLR-0.5LLR),        0,    63,   0x1P-63LLR),

	(USACCUM,   0XFFFFP-8UHK,                0.0UHK,                  8,    8,    0x1P-8UHK),
	(UACCUM,    0XFFFFFFFFP-16UK,            0.0UK,                   16,   16,   0x1P-16UK),
	(ULACCUM,   0XFFFFFFFFFFFFFFFFP-32ULK,   0.0ULK,                  32,   32,   0x1P-32ULK),
	(ULLACCUM,  0XFFFFFFFFFFFFFFFFP-32ULLK,  0.0ULLK,                 32,   32,   0x1P-32ULLK),

	(SACCUM,    0X7FFFP-7HK,                 (-0X1P7HK-0X1P7HK),      8,    7,    0x1P-7HK),
	(ACCUM,     0X7FFFFFFFP-15K,             (-0X1P15K-0X1P15K),      16,   15,   0x1P-15K),
	(LACCUM,    0X7FFFFFFFFFFFFFFFP-31LK,    (-0X1P31LK-0X1P31LK),    32,   31,   0x1P-31LK),
	(LLACCUM,   0X7FFFFFFFFFFFFFFFP-31LLK,   (-0X1P31LLK-0X1P31LLK),  32,   31,   0x1P-31LLK),
)~,` m4_dnl;
m4_take_compilers_or_default($1_MAX, $2)
m4_take_compilers_or_default($1_MIN, $3)
m4_take_compilers_or_default($1_IBIT, $4)
m4_take_compilers_or_default($1_FBIT, $5)
m4_take_compilers_or_default($1_EPSILON, $6)
~)m4_dnl m4_applyforeachdefine ;

#endif // _yIO_HAS_STDFIX
