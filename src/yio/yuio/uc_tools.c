/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _GNU_SOURCE  1
#include "uc_tools.h"
#include <unistr.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <limits.h>
#include <unistr.h>
#include <uniconv.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#define dbg(str, ...)  do{}while(0)
#ifndef dbg
#define dbg(str, ...)  fprintf(stderr, "%s:%3d: " str "\n", __func__, __LINE__, ##__VA_ARGS__)
#endif

uint8_t *u8_fgetc_u8(FILE *f, uint8_t *resultbuf, size_t *lengthp) {
	const bool dynamic = resultbuf == NULL;
	int err = 0;

	uint8_t *str = resultbuf;
	size_t strpos = 0;

	unsigned count = 1; // how many characters to read
	for (int ic; count != 0 && (ic = fgetc(f)) != EOF; --count) {
		dbg("READ: %#02x", ic);
		if (dynamic) {
			void *p = realloc(str, sizeof(*str) * (strpos + 1));
			if (p == NULL) {
				dbg("");
				err = -1;
				goto EXIT;
			}
			str = p;
		} else {
			if (*lengthp <= strpos) {
				dbg("");
				errno = ENOMEM;
				err = -1;
				goto EXIT;
			}
		}

		const unsigned char c = ic;
		if (strpos == 0) {
			if ((c & 0x80) == 0x00) {
				count = 1;
			} else if ((c & 0xE0) == 0xC0){
				count = 2;
			} else if ((c & 0xF0) == 0xE0){
				count = 3;
			} else if ((c & 0xF8) == 0xF0){
				count = 4;
			} else if ((c & 0xFC) == 0xF8){
				count = 5;
			} else if ((c & 0xFE) == 0xFC){
				count = 6;
			} else {
				dbg("%d %zu %u %#02x", __LINE__, strpos, count, c);
				errno = EILSEQ;
				err = -1;
				goto EXIT;
			}
		} else {
			if ((c & 0xC0) != 0x80) {
				dbg("%d", __LINE__);
				errno = EILSEQ;
				err = -1;
				goto EXIT;
			}
		}

		dbg("Add: %#02x count=%u strpos=%zu", c, count, strpos);
		str[strpos] = c;
		strpos++;
	}

	if (count != 0) {
		dbg("count != 0");
		errno = EINVAL;
		err = -1;
		goto EXIT;
	}

	if (u8_check(str, strpos) != NULL) {
		dbg("u8_check(str, strpos) != NULL: %d %s", __LINE__, str);
		errno = EILSEQ;
		err = -1;
		goto EXIT;
	}

	EXIT:
	if (err) {
		dbg("err '%.*s' %zu", (int)strpos, str ? "" : (char*)str, strpos);
		if (dynamic) {
			free(str);
		}
		str = NULL;
		*lengthp = 0;
	} else {
		dbg("!err '%.*s' %zu", (int)strpos, (char*)str, strpos);
		*lengthp = strpos;
	}
	return str;
}

uint32_t u32_fgetc_u8(FILE *f) {
	uint32_t ret;

	uint8_t u8[u8_max_codepoint_len];
	size_t u8length = sizeof(u8);
	if (u8_fgetc_u8(f, u8, &u8length) == NULL) {
		dbg("");
		ret = U32EOF;
		goto EXIT;
	}
	if (u8_to_u32(u8, u8length, &ret, (size_t[1]){1}) == NULL) {
		dbg("");
		ret = U32EOF;
		goto EXIT;
	}

	EXIT:
	return ret;
}

uint32_t u32_fgetc_w(FILE *f) {
	uint32_t ret = UCEOF;

	const wint_t c = fgetwc(f);
	if (c == WEOF) {
		goto EXIT;
	}

	// convert wide character to multibyte local string
	char s[MB_LEN_MAX + 1] = {0};
	mbstate_t mb = {0};
	const size_t wcrtomberr = wcrtomb(s, c, &mb);
	if (wcrtomberr == (size_t)-1) {
		goto EXIT;
	}

	// convert multibyte local string to UTF-32
	uint32_t *s32 = u32_strconv_from_locale(s);
	if (s32 == NULL) {
		goto EXIT;
	}

	ret = s32[0];

	free(s32);
	EXIT:
	return ret;
}

uint32_t *u32_str_fgets_w(uint32_t *s, size_t size, FILE *f) {
	uint32_t * const s0 = s;
	for (uint32_t c;
			(c = u32_fgetc_w(f)) != U32EOF &&
			c != '\n' &&
			size != 0; --size) {
		*s++ = c;
	}
	if (size) {
		*s = 0;
	}
	return s0;
}

ssize_t u32_str_fgetdelim_generic(uint32_t **lineptr, size_t *n, uint32_t delim,
		uint32_t (*u32_fgetc_generic)(FILE *stream), FILE *stream) {
	ssize_t ret = 0;
	size_t pos = 0;

	for (bool end = false; end == false; ) {
		uint32_t c = u32_fgetc_generic(stream);
		if (c == U32EOF || u32_cmp(&c, &delim, 1) == 0) {
			c = 0;
			end = true;
		}

		if (*n <= pos) {
			void *p = realloc(*lineptr, sizeof(**lineptr) * (pos + 1));
			if (p == NULL) {
				ret = -1;
				break;
			}
			*n = pos + 1;
			*lineptr = p;
		}

		if (pos == SSIZE_MAX) {
			ret = -1;
			break;
		}

		(*lineptr)[pos++] = c;
	}

	if (ret == 0) {
		*n = pos;
		ret = pos ? pos - 1 : pos;
	} else {
		free(*lineptr);
		*n = 0;
		*lineptr = NULL;
	}
	return ret;
}

uint8_t *u32_to_u8_str(uint32_t *s, size_t length) {
	size_t lengthp;
	uint8_t *ret = u32_to_u8(s, length, NULL, &lengthp);
	if (ret == NULL) {
		goto EXIT;
	}

	if (lengthp == SIZE_MAX) {
		free(ret);
		ret = NULL;
		goto EXIT;
	}

	void *p = realloc(ret, (lengthp + 1) * sizeof(*ret));
	if (p == NULL) {
		free(ret);
		ret = NULL;
		goto EXIT;
	}
	ret = p;

	ret[lengthp] = 0;

	EXIT:
	return ret;
}


