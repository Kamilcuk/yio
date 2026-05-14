/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef YIO_YIO_YIO_PRIVATE_H_
#define YIO_YIO_YIO_PRIVATE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "yio_public.h"
#include <stdbool.h>

#ifndef YIO_PRIVATE
#error YIO_PRIVATE is not defined, but include yio_private.h. This is internal header for internal use only.
#endif

/**
 * @def YIO_INIT_CAPACITY
 * @brief Initial capacity for dynamic string allocations and temporary stack buffers.
 *
 * 32 bytes is a reasonable trade-off: it is small enough to avoid significant memory
 * waste for very short strings, yet large enough to handle many common formatted
 * outputs (such as small integers, pointers, or short labels) and typical padding
 * requirements without immediate reallocation or excessive tiny writes.
 */
#ifdef __SDCC
#define YIO_INIT_CAPACITY  2
#else
#define YIO_INIT_CAPACITY  32
#endif

/**
 * @def YIO_GOLDEN_INCREASE
 * @brief Increase the size of a dynamically allocated array using a rational approximation of the golden ratio (1.625).
 *
 * Using a growth factor less than 2 (specifically around 1.5 to 1.618) allows for the reuse of
 * previously freed memory blocks. With a factor of 2, the next allocation is always larger than
 * the sum of all previous allocations, making it impossible to reuse the "hole" left behind by
 * previous blocks even if they were adjacent.
 *
 * Rational approximation: 13/8 = 1.625.
 *
 * @see https://stackoverflow.com/questions/1100311/what-is-the-ideal-growth-rate-for-a-dynamically-allocated-array
 */
#define YIO_GOLDEN_INCREASE(x) ((x) * 13 / 8)

/**
 * @brief Calculates the maximum number of decimal digits required to represent
 * a bit-precise integer of width x.
 * * Formula: ceil(x * log10(2))
 * This macro uses a rational approximation (146/485) to provide the correct
 * ceiling value for all x from 1 to 1024 without requiring floating point
 * math or massive look-up tables.
 * @param x The bit-width (e.g., from _BitInt(x)).
 * @return The maximum number of decimal characters needed (buffer size).
 * @see https://www.wolframalpha.com/input/?i=ceiling%28log_10%282%5Ex%29%29+for+x+%3D+1+to+256
 */
#define YIO_LOG10_POW2(x) ((x) <= 0 ? 0 : (((x) * 146 + 484) / 485))

/**
 * @def YIO_INT_STRLEN_BOUND
 * The number of digits needed to represent an int in base10 _without_ sign.
 */
#define YIO_INT_STRLEN_BOUND()   YIO_LOG10_POW2(sizeof(int) * CHAR_BIT)

/**
 * @def YIO_tolower
 * @brief Simple conversion to lowercase. Only for safe characters!
 */
static inline char YIO_tolower(char c) { return (char)((unsigned char)c | 32); }

/**
 * @def YIO_isupper
 * @brief Simple check if character is uppercase. Only for safe characters!
 */
static inline bool YIO_isupper(char c) { return (unsigned char)c >= 'A' && (unsigned char)c <= 'Z'; }

/**
 * @def YIO_isdigit
 * @brief Fastest check if a character is a digit.
 */
static inline bool YIO_isdigit(char c) { return (unsigned int)((unsigned char)c - '0') <= 9U; }

/**
 * @def YIO_isxdigit
 * @brief Fastest check if a character is a hex digit.
 */
static inline bool YIO_isxdigit(char c) {
	const unsigned char uc = (unsigned char)c;
	return YIO_isdigit(c) || ( (unsigned int)(YIO_tolower((char)uc) - 'a') <= (unsigned int)('f' - 'a') );
}

static const char *const YIO_digit_to_HEX = "0123456789ABCDEF";
static const char *const YIO_digit_to_hex = "0123456789abcdef";
static const char *YIO_digit_to_hexs(bool lower) {
	return lower ? YIO_digit_to_hex : YIO_digit_to_HEX;
}

/**
 * @def YIO_ANYEQ
 * @brief Checks if a value is equal to any of the arguments.
 */
#define YIO_GET_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define YIO_COUNT_ARGS(...) YIO_GET_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

static inline bool YIO_anyeq1(unsigned char v, unsigned char a) { return v == a; }
static inline bool YIO_anyeq2(unsigned char v, unsigned char a, unsigned char b) { return v == a || v == b; }
static inline bool YIO_anyeq3(unsigned char v, unsigned char a, unsigned char b, unsigned char c) { return v == a || v == b || v == c; }
static inline bool YIO_anyeq4(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d) { return v == a || v == b || v == c || v == d; }
static inline bool YIO_anyeq5(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e) { return v == a || v == b || v == c || v == d || v == e; }
static inline bool YIO_anyeq6(unsigned char v, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f) { return v == a || v == b || v == c || v == d || v == e || v == f; }

#define YIO_ANYEQ(v, ...) YIO_XCONCAT(YIO_anyeq, YIO_COUNT_ARGS(__VA_ARGS__))(v, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_PRIVATE_H_
