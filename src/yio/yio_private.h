/**
 * @file yio_private.h
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#pragma once
#include "yio.h"
#include "yio_fmt.h"
#include "yio_ctx.h"
#include "intprops.h"
#include <string.h>

#define _yIO_STATIC_ASSERT(x)  ((void)sizeof(struct _yIO_sTaTiS_aSsErT{int _yIO_sTaTiS_aSsErT : (x)?1:-1;}))

// https://www.wolframalpha.com/input/?i=ceiling%28log_10%282%5Ex%29%29+for+x+%3D+1+to+256
#define _yIO_LOG10_POW2(x) ( \
		(x) == 1 ? 1 : 		(x) == 2 ? 1 : 		(x) == 3 ? 1 : 		(x) == 4 ? 2 : 		(x) == 5 ? 2 : \
		(x) == 6 ? 2 : 		(x) == 7 ? 3 : 		(x) == 8 ? 3 : 		(x) == 9 ? 3 : 		(x) == 10 ? 4 : \
		(x) == 11 ? 4 : 		(x) == 12 ? 4 : 		(x) == 13 ? 4 : 		(x) == 14 ? 5 : 		(x) == 15 ? 5 : \
		(x) == 16 ? 5 : 		(x) == 17 ? 6 : 		(x) == 18 ? 6 : 		(x) == 19 ? 6 : 		(x) == 20 ? 7 : \
		(x) == 21 ? 7 : 		(x) == 22 ? 7 : 		(x) == 23 ? 7 : 		(x) == 24 ? 8 : 		(x) == 25 ? 8 : \
		(x) == 26 ? 8 : 		(x) == 27 ? 9 : 		(x) == 28 ? 9 : 		(x) == 29 ? 9 : 		(x) == 30 ? 10 : \
		(x) == 31 ? 10 : 		(x) == 32 ? 10 : 		(x) == 33 ? 10 : 		(x) == 34 ? 11 : 		(x) == 35 ? 11 : \
		(x) == 36 ? 11 : 		(x) == 37 ? 12 : 		(x) == 38 ? 12 : 		(x) == 39 ? 12 : 		(x) == 40 ? 13 : \
		(x) == 41 ? 13 : 		(x) == 42 ? 13 : 		(x) == 43 ? 13 : 		(x) == 44 ? 14 : 		(x) == 45 ? 14 : \
		(x) == 46 ? 14 : 		(x) == 47 ? 15 : 		(x) == 48 ? 15 : 		(x) == 49 ? 15 : 		(x) == 50 ? 16 : \
		(x) == 51 ? 16 : 		(x) == 52 ? 16 : 		(x) == 53 ? 16 : 		(x) == 54 ? 17 : 		(x) == 55 ? 17 : \
		(x) == 56 ? 17 : 		(x) == 57 ? 18 : 		(x) == 58 ? 18 : 		(x) == 59 ? 18 : 		(x) == 60 ? 19 : \
		(x) == 61 ? 19 : 		(x) == 62 ? 19 : 		(x) == 63 ? 19 : 		(x) == 64 ? 20 : 		(x) == 65 ? 20 : \
		(x) == 66 ? 20 : 		(x) == 67 ? 21 : 		(x) == 68 ? 21 : 		(x) == 69 ? 21 : 		(x) == 70 ? 22 : \
		(x) == 71 ? 22 : 		(x) == 72 ? 22 : 		(x) == 73 ? 22 : 		(x) == 74 ? 23 : 		(x) == 75 ? 23 : \
		(x) == 76 ? 23 : 		(x) == 77 ? 24 : 		(x) == 78 ? 24 : 		(x) == 79 ? 24 : 		(x) == 80 ? 25 : \
		(x) == 81 ? 25 : 		(x) == 82 ? 25 : 		(x) == 83 ? 25 : 		(x) == 84 ? 26 : 		(x) == 85 ? 26 : \
		(x) == 86 ? 26 : 		(x) == 87 ? 27 : 		(x) == 88 ? 27 : 		(x) == 89 ? 27 : 		(x) == 90 ? 28 : \
		(x) == 91 ? 28 : 		(x) == 92 ? 28 : 		(x) == 93 ? 28 : 		(x) == 94 ? 29 : 		(x) == 95 ? 29 : \
		(x) == 96 ? 29 : 		(x) == 97 ? 30 : 		(x) == 98 ? 30 : 		(x) == 99 ? 30 : 		(x) == 100 ? 31 : \
		(x) == 101 ? 31 : 		(x) == 102 ? 31 : 		(x) == 103 ? 32 : 		(x) == 104 ? 32 : 		(x) == 105 ? 32 : \
		(x) == 106 ? 32 : 		(x) == 107 ? 33 : 		(x) == 108 ? 33 : 		(x) == 109 ? 33 : 		(x) == 110 ? 34 : \
		(x) == 111 ? 34 : 		(x) == 112 ? 34 : 		(x) == 113 ? 35 : 		(x) == 114 ? 35 : 		(x) == 115 ? 35 : \
		(x) == 116 ? 35 : 		(x) == 117 ? 36 : 		(x) == 118 ? 36 : 		(x) == 119 ? 36 : 		(x) == 120 ? 37 : \
		(x) == 121 ? 37 : 		(x) == 122 ? 37 : 		(x) == 123 ? 38 : 		(x) == 124 ? 38 : 		(x) == 125 ? 38 : \
		(x) == 126 ? 38 : 		(x) == 127 ? 39 : 		(x) == 128 ? 39 : 		(x) == 129 ? 39 : 		(x) == 130 ? 40 : \
		(x) == 131 ? 40 : 		(x) == 132 ? 40 : 		(x) == 133 ? 41 : 		(x) == 134 ? 41 : 		(x) == 135 ? 41 : \
		(x) == 136 ? 41 : 		(x) == 137 ? 42 : 		(x) == 138 ? 42 : 		(x) == 139 ? 42 : 		(x) == 140 ? 43 : \
		(x) == 141 ? 43 : 		(x) == 142 ? 43 : 		(x) == 143 ? 44 : 		(x) == 144 ? 44 : 		(x) == 145 ? 44 : \
		(x) == 146 ? 44 : 		(x) == 147 ? 45 : 		(x) == 148 ? 45 : 		(x) == 149 ? 45 : 		(x) == 150 ? 46 : \
		(x) == 151 ? 46 : 		(x) == 152 ? 46 : 		(x) == 153 ? 47 : 		(x) == 154 ? 47 : 		(x) == 155 ? 47 : \
		(x) == 156 ? 47 : 		(x) == 157 ? 48 : 		(x) == 158 ? 48 : 		(x) == 159 ? 48 : 		(x) == 160 ? 49 : \
		(x) == 161 ? 49 : 		(x) == 162 ? 49 : 		(x) == 163 ? 50 : 		(x) == 164 ? 50 : 		(x) == 165 ? 50 : \
		(x) == 166 ? 50 : 		(x) == 167 ? 51 : 		(x) == 168 ? 51 : 		(x) == 169 ? 51 : 		(x) == 170 ? 52 : \
		(x) == 171 ? 52 : 		(x) == 172 ? 52 : 		(x) == 173 ? 53 : 		(x) == 174 ? 53 : 		(x) == 175 ? 53 : \
		(x) == 176 ? 53 : 		(x) == 177 ? 54 : 		(x) == 178 ? 54 : 		(x) == 179 ? 54 : 		(x) == 180 ? 55 : \
		(x) == 181 ? 55 : 		(x) == 182 ? 55 : 		(x) == 183 ? 56 : 		(x) == 184 ? 56 : 		(x) == 185 ? 56 : \
		(x) == 186 ? 56 : 		(x) == 187 ? 57 : 		(x) == 188 ? 57 : 		(x) == 189 ? 57 : 		(x) == 190 ? 58 : \
		(x) == 191 ? 58 : 		(x) == 192 ? 58 : 		(x) == 193 ? 59 : 		(x) == 194 ? 59 : 		(x) == 195 ? 59 : \
		(x) == 196 ? 60 : 		(x) == 197 ? 60 : 		(x) == 198 ? 60 : 		(x) == 199 ? 60 : 		(x) == 200 ? 61 : \
		(x) == 201 ? 61 : 		(x) == 202 ? 61 : 		(x) == 203 ? 62 : 		(x) == 204 ? 62 : 		(x) == 205 ? 62 : \
		(x) == 206 ? 63 : 		(x) == 207 ? 63 : 		(x) == 208 ? 63 : 		(x) == 209 ? 63 : 		(x) == 210 ? 64 : \
		(x) == 211 ? 64 : 		(x) == 212 ? 64 : 		(x) == 213 ? 65 : 		(x) == 214 ? 65 : 		(x) == 215 ? 65 : \
		(x) == 216 ? 66 : 		(x) == 217 ? 66 : 		(x) == 218 ? 66 : 		(x) == 219 ? 66 : 		(x) == 220 ? 67 : \
		(x) == 221 ? 67 : 		(x) == 222 ? 67 : 		(x) == 223 ? 68 : 		(x) == 224 ? 68 : 		(x) == 225 ? 68 : \
		(x) == 226 ? 69 : 		(x) == 227 ? 69 : 		(x) == 228 ? 69 : 		(x) == 229 ? 69 : 		(x) == 230 ? 70 : \
		(x) == 231 ? 70 : 		(x) == 232 ? 70 : 		(x) == 233 ? 71 : 		(x) == 234 ? 71 : 		(x) == 235 ? 71 : \
		(x) == 236 ? 72 : 		(x) == 237 ? 72 : 		(x) == 238 ? 72 : 		(x) == 239 ? 72 : 		(x) == 240 ? 73 : \
		(x) == 241 ? 73 : 		(x) == 242 ? 73 : 		(x) == 243 ? 74 : 		(x) == 244 ? 74 : 		(x) == 245 ? 74 : \
		(x) == 246 ? 75 : 		(x) == 247 ? 75 : 		(x) == 248 ? 75 : 		(x) == 249 ? 75 : 		(x) == 250 ? 76 : \
		(x) == 251 ? 76 : 		(x) == 252 ? 76 : 		(x) == 253 ? 77 : 		(x) == 254 ? 77 : 		(x) == 255 ? 77 : \
		(x) == 256 ? 78 : 				\
		-1)

#define _yIO_MAXDIGITS_IN(type)     _yIO_LOG10_POW2(sizeof(type) * CHAR_BIT)

#define _yIO_MAXHEXDIGITS_IN(type)  (sizeof(type) * CHAR_BIT / 4)

#define _yIO_INTEGERS_LIST2() \
		(short, short), \
		(ushort, unsigned short), \
		(int, int), \
		(uint, unsigned int), \
		(long, long), \
		(ulong, unsigned long), \
		(llong, long long), \
		(ullong, unsigned long long), \
		(__int128, __int128), \
		(u__int128, unsigned __int128)

#define _yIO_TYPES_LIST2() \
		(schar, signed char), \
		(uchar, unsigned char), \
		_yIO_INTEGERS_LIST2()


void _yIO__test_failed(const char *expr, const char *file, int line,
		const char *fmt, ...);

#define _yIO_TEST(expr) _yIO_TEST_MSG(expr, "")
#define _yIO_TEST_MSG(expr, str, ...)  do{ \
		if (!(expr)) { \
			_yIO__test_failed(#expr, __FILE__, __LINE__, str, ##__VA_ARGS__); \
		} \
}while(0)

#define _yIO_dbgln(str, ...)  do{ \
		fflush(stdout); \
		fprintf(stderr, "%s:%d: " str "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)

const char *_yIO_char_to_printstr_in(char dest[5], unsigned char c);
#define _yIO_printc(c)  _yIO_char_to_printstr_in((char[5]){0}, c)



