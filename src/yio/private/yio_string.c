/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_string.h"
#include "private.h"
#include "../yio_error.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int YYIO_string_reserve(YYIO_string *t, size_t newsize) {
	const size_t size = YYIO_string_capacity(t);
	if (newsize <= size) return 0;
#if YIO_USE_MALLOC
	const size_t len = YYIO_string_len(t);
	const bool dynamic = YYIO_string_is_dynamic(t);
	void *const p = realloc(dynamic ? t->h.ptr : NULL, newsize);
	if (p == NULL) return YIO_ERROR_ENOMEM;
	if (!dynamic) {
		memcpy(p, t->s.buf, len);
	}
	t->h.ptr = p;
	t->h.len = len;
	t->info = (newsize << 1) | 1;
	return 0;
#else
	return YIO_ERROR_ENOMEM;
#endif
}

int YYIO_string_reserve_more(YYIO_string *t) {
	const size_t size = YYIO_string_capacity(t);
	const size_t init_chunk = YYIO_INIT_CAPACITY;
	const size_t newsizecalc = YYIO_GOLDEN_INCREASE(size);
	const size_t newsize = newsizecalc > init_chunk ? newsizecalc : init_chunk;
	return YYIO_string_reserve(t, newsize);
}

int YYIO_string_putsn(YYIO_string *t, const char *ptr, size_t size) {
	const size_t current_len = YYIO_string_len(t);
	const size_t needed = current_len + size;
	if (YYIO_string_capacity(t) < needed) {
		const size_t cap = YYIO_string_capacity(t);
		const size_t init_chunk = YYIO_INIT_CAPACITY;
		size_t new_cap = YYIO_GOLDEN_INCREASE(cap);
		if (new_cap < init_chunk) new_cap = init_chunk;
		if (new_cap < needed) new_cap = needed;
		const int err = YYIO_string_reserve(t, new_cap);
		if (err) return err;
	}
	memcpy(YYIO_string_data(t) + current_len, ptr, size);
	YYIO_string_set_used(t, needed);
	return 0;
}

static
int YYIO_string_yprintf_cb(void *ptr, const char *data, size_t count) {
	YYIO_string *o = ptr;
	return YYIO_string_putsn(o, data, count);
}

int YYIO_string_yprintf_in(YYIO_string *t, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yio_vbprintf(YYIO_string_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) return err;
	return 0;
}
