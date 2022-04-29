/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef _yIO_YIO_YIO_PRIVATE_H_
#define _yIO_YIO_YIO_PRIVATE_H_
#include "yio_public.h"
#include <string.h>

#ifndef _yIO_PRIVATE
#error _yIO_PRIVATE
#endif

// https://www.wolframalpha.com/input/?i=ceiling%28log_10%282%5Ex%29%29+for+x+%3D+1+to+256
#define _yIO_LOG10_POW2(x) ( \
		(x) < 3   ? 1  : (x) < 6   ? 2  : (x) < 9   ? 3  : (x) < 13  ? 4  : (x) < 16  ? 5  : \
		(x) < 19  ? 6  : (x) < 23  ? 7  : (x) < 26  ? 8  : (x) < 29  ? 9  : (x) < 33  ? 10 : \
		(x) < 36  ? 11 : (x) < 39  ? 12 : (x) < 43  ? 13 : (x) < 46  ? 14 : (x) < 49  ? 15 : \
		(x) < 53  ? 16 : (x) < 56  ? 17 : (x) < 59  ? 18 : (x) < 63  ? 19 : (x) < 66  ? 20 : \
		(x) < 69  ? 21 : (x) < 73  ? 22 : (x) < 76  ? 23 : (x) < 79  ? 24 : (x) < 83  ? 25 : \
		(x) < 86  ? 26 : (x) < 89  ? 27 : (x) < 93  ? 28 : (x) < 96  ? 29 : (x) < 99  ? 30 : \
		(x) < 102 ? 31 : (x) < 106 ? 32 : (x) < 109 ? 33 : (x) < 112 ? 34 : (x) < 116 ? 35 : \
		(x) < 119 ? 36 : (x) < 122 ? 37 : (x) < 126 ? 38 : (x) < 129 ? 39 : (x) < 132 ? 40 : \
		(x) < 136 ? 41 : (x) < 139 ? 42 : (x) < 142 ? 43 : (x) < 146 ? 44 : (x) < 149 ? 45 : \
		(x) < 152 ? 46 : (x) < 156 ? 47 : (x) < 159 ? 48 : (x) < 162 ? 49 : (x) < 166 ? 50 : \
		(x) < 169 ? 51 : (x) < 172 ? 52 : (x) < 176 ? 53 : (x) < 179 ? 54 : (x) < 182 ? 55 : \
		(x) < 186 ? 56 : (x) < 189 ? 57 : (x) < 192 ? 58 : (x) < 195 ? 59 : (x) < 199 ? 60 : \
		(x) < 202 ? 61 : (x) < 205 ? 62 : (x) < 209 ? 63 : (x) < 212 ? 64 : (x) < 215 ? 65 : \
		(x) < 219 ? 66 : (x) < 222 ? 67 : (x) < 225 ? 68 : (x) < 229 ? 69 : (x) < 232 ? 70 : \
		(x) < 235 ? 71 : (x) < 239 ? 72 : (x) < 242 ? 73 : (x) < 245 ? 74 : (x) < 249 ? 75 : \
		(x) < 252 ? 76 : (x) < 255 ? 77 : \
		-100000)

/**
 * @def _yIO_INT_STRLEN_BOUND
 * The number of digits needed to represent an int in base10 _without_ sign.
 */
#define _yIO_INT_STRLEN_BOUND()   _yIO_LOG10_POW2(sizeof(int) - 1)


/**
 * @def dbgln
 * small debugging utility
 */
#define dbgln(str, ...)  do{ \
		fflush(0); \
		fprintf(stderr, "%s:%d: " str "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)

const char *_yIO_char_to_printstr_in(char dest[5], unsigned char c);
#define _yIO_printc(c)  _yIO_char_to_printstr_in((char[5]){0}, c)

/**
 * @def _yIO_ERROR(ENUM, DESC)
 * @param ENUM The suffix to YIO_ERROR_* enum name.
 * @param DESC The description of the errors
 * @short Is used to automatically parse and register an error code with description.
 */
#define _yIO_ERROR(ENUM, DESC)  ENUM

#endif /* _yIO_YIO_YIO_PRIVATE_H_ */
