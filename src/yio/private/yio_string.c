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
int YIO_string_reserve(YIO_string *t, size_t newsize) {
	const size_t size = YIO_string_capacity(t);
	if (newsize <= size) return 0;
	const size_t len = YIO_string_len(t);
	const bool dynamic = YIO_string_is_dynamic(t);
	void *const p = realloc(dynamic ? t->h.ptr : NULL, newsize);
	if (p == NULL) return YIO_ERROR_ENOMEM;
	if (!dynamic) memcpy(p, t->buf, len);
	t->h.ptr = p;
	t->h.len = len;
	t->info = (newsize << 1) | 1;
	return 0;
}

int YIO_string_reserve_more(YIO_string *t, size_t min_add) {
    const size_t current_cap = YIO_string_capacity(t);
    const size_t needed = current_cap + min_add;
    // Calculate expansion using golden ratio or initial chunk
    size_t new_cap = YIO_GOLDEN_INCREASE(needed);
    if (new_cap < YIO_INIT_CAPACITY) new_cap = YIO_INIT_CAPACITY;
  	if (new_cap < YIO_SSO_SIZE) new_cap = YIO_SSO_SIZE;
    if (new_cap < needed) new_cap = needed;
    return YIO_string_reserve(t, new_cap);
}
#endif

int YIO_string_putsn(YIO_string *t, const char *ptr, size_t size) {
	const size_t current_len = YIO_string_len(t);
	const size_t needed = current_len + size;
	const size_t cap = YIO_string_capacity(t);
	if (cap < needed) {
    const int err = YIO_string_reserve_more(t, size);
    if (err) return err;
	}
	memcpy(YIO_string_data(t) + current_len, ptr, size);
	YIO_string_set_used(t, needed);
	return 0;
}
