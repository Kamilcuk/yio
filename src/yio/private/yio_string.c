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

#if YIO_ENABLE_MALLOC
int YYIO_string_reserve(YYIO_string *t, size_t newsize) {
	const size_t size = YYIO_string_capacity(t);
	if (newsize <= size) return 0;
	const size_t len = YYIO_string_len(t);
	const bool dynamic = YYIO_string_is_dynamic(t);
	void *const p = realloc(dynamic ? t->h.ptr : NULL, newsize);
	if (p == NULL) return YIO_ERROR_ENOMEM;
	if (!dynamic) memcpy(p, t->buf, len);
	t->h.ptr = p;
	t->h.len = len;
	t->info = (newsize << 1) | 1;
	return 0;
}

int YYIO_string_reserve_more(YYIO_string *t, size_t min_add) {
    const size_t current_cap = YYIO_string_capacity(t);
    const size_t needed = current_cap + min_add;
    // Calculate expansion using golden ratio or initial chunk
    size_t new_cap = YYIO_GOLDEN_INCREASE(needed);
    if (new_cap < YYIO_INIT_CAPACITY) new_cap = YYIO_INIT_CAPACITY;
  	if (new_cap < YYIO_SSO_SIZE) new_cap = YYIO_SSO_SIZE;
    if (new_cap < needed) new_cap = needed;
    return YYIO_string_reserve(t, new_cap);
}
#endif

int YYIO_string_putsn(YYIO_string *t, const char *ptr, size_t size) {
	const size_t current_len = YYIO_string_len(t);
	const size_t needed = current_len + size;
	const size_t cap = YYIO_string_capacity(t);
	if (cap < needed) {
    const int err = YYIO_string_reserve_more(t, size);
    if (err) return err;
	}
	memcpy(YYIO_string_data(t) + current_len, ptr, size);
	YYIO_string_set_used(t, needed);
	return 0;
}
