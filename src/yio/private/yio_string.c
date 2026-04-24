/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_string.h"
#include "../yio_error.h"
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int YYIO_string_reserve(YYIO_string *t, size_t newsize) {
	const size_t size = YYIO_string_capacity(t);
	if (newsize <= size) {
		return 0;
	}
	const size_t len = YYIO_string_len(t);
	const bool dynamic = YYIO_string_is_dynamic(t);
	
	void *const p = realloc(dynamic ? t->h.ptr : NULL, newsize);
	if (p == NULL) {
		return YIO_ERROR_ENOMEM;
	}
	if (!dynamic) {
		memcpy(p, t->s.buf, len);
	}
	t->h.ptr = p;
	t->h.len = len;
	t->h.info = (newsize << 1) | 1;
	return 0;
}

int YYIO_string_reserve_more(YYIO_string *t) {
	const size_t size = YYIO_string_capacity(t);
	const size_t YYIO_string_init_chunk = 32;
	assert(size < SIZE_MAX / 52);
	// golden ratio
	const size_t newsizecalc = size * 52 / 32;
	const size_t newsize = newsizecalc > YYIO_string_init_chunk ? newsizecalc : YYIO_string_init_chunk;
	return YYIO_string_reserve(t, newsize);
}

int YYIO_string_putsn(YYIO_string *t, const char *ptr, size_t size) {
	const size_t current_len = YYIO_string_len(t);
	while (YYIO_string_free_size(t) < size) {
		const int err = YYIO_string_reserve_more(t);
		if (err) return err;
	}
	memcpy(YYIO_string_data(t) + current_len, ptr, size);
	YYIO_string_set_used(t, current_len + size);
	return 0;
}

static
int YYIO_string_yprintf_cb(void *ptr, const char *data, size_t count) {
	YYIO_string *o = ptr;
	return YYIO_string_putsn(o, data, count);
}

int YYIO_string_yprintf_in(YYIO_string *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yio_vbprintf(YYIO_string_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) {
		return err;
	}
	return 0;
}

bool YYIO_string_remove_trailing_zeros_and_comma(YYIO_string *t) {
	bool fractional_part_removed = false;
	const size_t len = YYIO_string_len(t);
	if (len == 0) return false;
	char * const data = YYIO_string_data(t);
	char *p = data + len - 1;
	// there is dot, so the following loop will always stop
	while (p != data && *p == '0') {
		--p;
	}
	assert(isdigit((unsigned char)*p) || *p == '.' || strchr("abcdefABCDEF", *p));
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	YYIO_string_set_used(t, (size_t)(p - data));
	return fractional_part_removed;
}
