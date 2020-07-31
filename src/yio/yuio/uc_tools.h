/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_UTF_TOOLS_H_
#define YIO_UTF_TOOLS_H_

#include <uniconv.h>
#include <unistr.h>
#include <stdio.h>
#include <wchar.h>
#include <sys/types.h>

static const ucs4_t UCEOF = 0xFFFFFFFF;

static const uint32_t U32EOF = UCEOF;

static const size_t u8_max_codepoint_len = 6;

/**
 * Reads UTF-8 character using fgetc
 * @param f
 * @param resultbuf
 * @param lengthp
 * @return Upon failure, -1 is returned for an invalid sequence of units,
 *         -2 is returned for an incomplete sequence of units.
 */
uint8_t *u8_fgetc_u8(FILE *f, uint8_t *resultbuf, size_t *lengthp);

/**
 * Reads UTF-8 character using fgetc and converts it to UTF-32
 * @param f
 * @return UTF-32 character
 */
uint32_t u32_fgetc_u8(FILE *f);

/**
 * Reads UTF-32 character using fgetwc
 * @param f
 * @return UTF-32 character
 */
uint32_t u32_fgetc_w(FILE *f);

/**
 * Reads UTF-32 string using u32_fgetc_w
 * @param s
 * @param size
 * @param f
 * @return UTF-32 character or NULL
 */
uint32_t *u32_str_fgets_w(uint32_t *s, size_t size, FILE *f);

/**
 * Reads UTF-32 string like GNU getline
 * using a generic callback that reads single UTF-32 chare *
 * @param lineptr
 * @param n
 * @param delim
 * @param u32_fgetc_generic
 * @param stream
 * @return
 */
ssize_t u32_str_fgetdelim_generic(uint32_t **lineptr, size_t *n, uint32_t delim,
		uint32_t (*u32_fgetc_generic)(FILE *stream), FILE *stream);

static inline
ssize_t u32_str_fgetdelim_w(uint32_t **lineptr, size_t *n, uint32_t delim, FILE *stream) {
	return u32_str_fgetdelim_generic(lineptr, n, delim, u32_fgetc_w, stream);
}

static inline
ssize_t u32_str_fgetline_w(uint32_t **lineptr, size_t *n, FILE *stream) {
	return u32_str_fgetdelim_w(lineptr, n, U'\n', stream);
}

static inline
ssize_t u32_str_fgetdelim_u8(uint32_t **lineptr, size_t *n, uint32_t delim, FILE *stream) {
	return u32_str_fgetdelim_generic(lineptr, n, delim, u32_fgetc_u8, stream);
}

static inline
ssize_t u32_str_fgetline_u8(uint32_t **lineptr, size_t *n, FILE *stream) {
	return u32_str_fgetdelim_u8(lineptr, n, '\n', stream);
}

/**
 * Convert an UTF-32 string of size length to UTF-8 string
 * @param s
 * @param length
 * @return UTF-8 zero terminated string
 */
uint8_t *u32_to_u8_str(uint32_t *s, size_t length);

/**
 * Convert UTF-32 string to UTF-8 string
 * @param s
 * @return UTF-8 zero terminated string or NULL
 */
static inline
uint8_t *u32_str_to_u8_str(uint32_t *s) {
	return u32_to_u8_str(s, u32_strlen(s));
}

/**
 * Convet UTF-32 character to UTF-8 string
 * @param c
 * @return UTF-8 zero terminated string or NULL
 */
static inline
uint8_t *u32_to_u8_c(uint32_t c) {
	return u32_to_u8_str(&c, 1);
}

#endif /* YIO_UTF_TOOLS_H_ */
