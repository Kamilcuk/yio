/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_buf.h"
#include "private.h"
#include "../yio_error.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#if YIO_ENABLE_MALLOC
int YIO_buf_reserve(YIO_buf *t, size_t newsize) {
	const size_t size = YIO_buf_capacity(t);
	if (newsize <= size) return 0;
	newsize |= 1;
	const size_t len = YIO_buf_len(t);
	const bool dynamic = YIO_buf_is_dynamic(t);
	void *const p = realloc(dynamic ? t->h.ptr : NULL, newsize);
	if (p == NULL) return YIO_ERROR_ENOMEM;
	if (!dynamic) memcpy(p, t->buf, len);
	t->h.ptr = p;
	t->h.len = len;
	t->info = newsize;
	return 0;
}

int YIO_buf_reserve_more(YIO_buf *t, size_t min_add) {
    const size_t current_cap = YIO_buf_capacity(t);
    if (min_add > SIZE_MAX - current_cap) return YIO_ERROR_ENOMEM;
    const size_t needed = current_cap + min_add;
    // Calculate expansion using golden ratio or initial chunk
    size_t new_cap = YIO_GOLDEN_INCREASE(needed);
    if (new_cap < YIO_INIT_CAPACITY) new_cap = YIO_INIT_CAPACITY;
  	if (new_cap < YIO_SSO_SIZE) new_cap = YIO_SSO_SIZE;
    if (new_cap < needed) new_cap = needed;
    return YIO_buf_reserve(t, new_cap);
}
#endif

int YIO_buf_putsn(YIO_buf *t, const char *ptr, size_t size) {
	const size_t current_len = YIO_buf_len(t);
	if (size > SIZE_MAX - current_len) return YIO_ERROR_ENOMEM;
	const size_t needed = current_len + size;
	const size_t cap = YIO_buf_capacity(t);
	if (cap < needed) {
    const int err = YIO_buf_reserve_more(t, size);
    if (err) return err;
	}
	memcpy(YIO_buf_data(t) + current_len, ptr, size);
	YIO_buf_set_used(t, needed);
	return 0;
}
