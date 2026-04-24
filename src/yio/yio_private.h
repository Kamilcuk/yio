/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YYIO_YIO_YIO_PRIVATE_H_
#define YYIO_YIO_YIO_PRIVATE_H_
#include "yio_public.h"
#include <string.h>
#include <stdbool.h>

#ifndef YYIO_PRIVATE
#error YYIO_PRIVATE
#endif

// https://www.wolframalpha.com/input/?i=ceiling%28log_10%282%5Ex%29%29+for+x+%3D+1+to+256
#define YYIO_LOG10_POW2(x) ( \
		(x) < 3   ? 1  : (x) < 6   ? 2  : (x) < 9   ? 3  : (x) < 13  ? 4  : (x) < 16  ? 5  : \
		(x) < 19  ? 6  : (x) < 23  ? 7  : (x) < 26  ? 8  : (x) < 29  ? 9  : (x) < 33  ? 10 : \
		(x) < 36  ? 11 : (x) < 39  ? 12 : (x) < 43  ? 13 : (x) < 46  ? 14 : (x) < 49  ? 15 : \
		(x) < 53  ? 16 : (x) < 56  ? 17 : (x) < 59  ? 18 : (x) < 63  ? 19 : (x) < 66  ? 20 : \
		(x) < 69  ? 21 : (x) < 73  ? 22 : (x) < 76  ? 23 : (x) < 79  ? 24 : (x) < 83  ? 25 : \
		(x) < 86  ? 26 : (x) < 89  ? 27 : (x) < 93  ? 28 : (x) < 96  ? 29 : (x) < 99  ? 30 : \
		(x) < 102 ? 31 : (x) < 106 ? 32 : (x) < 109 ? 33 : (x) < 112 ? 34 : (x) < 116 ? 35 : \
		(x) < 119 ? 36 : (x) < 122 ? 37 : (x) < 126 ? 38 : (x) < 129 ? 39 : (x) < 132 ? 40 : \
		-100000)

/*
		(x) < 136 ? 41 : (x) < 139 ? 42 : (x) < 142 ? 43 : (x) < 146 ? 44 : (x) < 149 ? 45 : \
		(x) < 152 ? 46 : (x) < 156 ? 47 : (x) < 159 ? 48 : (x) < 162 ? 49 : (x) < 166 ? 50 : \
		(x) < 169 ? 51 : (x) < 172 ? 52 : (x) < 176 ? 53 : (x) < 179 ? 54 : (x) < 182 ? 55 : \
		(x) < 186 ? 56 : (x) < 189 ? 57 : (x) < 192 ? 58 : (x) < 195 ? 59 : (x) < 199 ? 60 : \
		(x) < 202 ? 61 : (x) < 205 ? 62 : (x) < 209 ? 63 : (x) < 212 ? 64 : (x) < 215 ? 65 : \
		(x) < 219 ? 66 : (x) < 222 ? 67 : (x) < 225 ? 68 : (x) < 229 ? 69 : (x) < 232 ? 70 : \
		(x) < 235 ? 71 : (x) < 239 ? 72 : (x) < 242 ? 73 : (x) < 245 ? 74 : (x) < 249 ? 75 : \
		(x) < 252 ? 76 : (x) < 255 ? 77 : \

*/

/**
 * @def YYIO_INT_STRLEN_BOUND
 * The number of digits needed to represent an int in base10 _without_ sign.
 */
#define YYIO_INT_STRLEN_BOUND()   YYIO_LOG10_POW2(sizeof(int) * CHAR_BIT)


/**
 * @def dbgln
 * small debugging utility
 */
#if YYIO_HAS_UNISTRING
#include <unistdio.h>
#define DBGPRINTF  ulc_fprintf
#else
#define DBGPRINTF  fprintf
#endif

#define dbgln(str, ...)  do{ \
		fflush(0); \
		DBGPRINTF(stderr, "\033[91m""%s:%d: " str "\033[0m""\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)

/**
 * @def YYIO_ERROR(ENUM, DESC)
 * @param ENUM The suffix to YIO_ERROR_* enum name.
 * @param DESC The description of the errors
 * @short Is used to automatically parse and register an error code with description.
 */
#define YYIO_ERROR(ENUM, DESC)  ENUM

/**
 * @def YYIO_ISDIGIT
 * @brief Fastest check if a character is a digit.
 */
#define YYIO_ISDIGIT(c) ((unsigned char)(c) - '0' <= 9u)

/**
 * @def YYIO_ISXDIGIT
 * @brief Fastest check if a character is a hex digit.
 */
#define YYIO_ISXDIGIT(c) (YYIO_ISDIGIT(c) || YYIO_ANYEQ(((unsigned char)(c) | 32), 'a', 'b', 'c', 'd', 'e', 'f'))

/**
 * @def YYIO_ANYEQ
 * @brief Checks if a value is equal to any of the arguments.
 */
#define YYIO_GET_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define YYIO_COUNT_ARGS(...) YYIO_GET_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

static inline bool YYIO_anyeq1(unsigned char v, unsigned char a) { return v == a; }
static inline bool YYIO_anyeq2(unsigned char v, unsigned char a, unsigned char b) { return v == a || v == b; }
static inline bool YYIO_anyeq3(unsigned char v, unsigned char a, unsigned char b, unsigned char c) { return v == a || v == b || v == c; }
static inline bool YYIO_anyeq4(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d) { return v == a || v == b || v == c || v == d; }
static inline bool YYIO_anyeq5(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e) { return v == a || v == b || v == c || v == d || v == e; }
static inline bool YYIO_anyeq6(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f) { return v == a || v == b || v == c || v == d || v == e || v == f; }

#define YYIO_ANYEQ(v, ...) YYIO_XCONCAT(YYIO_anyeq, YYIO_COUNT_ARGS(__VA_ARGS__))(v, __VA_ARGS__)

#endif /* YYIO_YIO_YIO_PRIVATE_H_ */
