/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_syncline(1)m4_dnl;
#include "yio_res.h"
#include "../yio_error.h"
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

_yIO_res *_yIO_res_init(_yIO_res *t, char **resultp, size_t *lengthp) {
	assert(resultp != NULL);
	assert(lengthp != NULL);
	const bool is_dynamic = *resultp == NULL || *lengthp == 0;
	_yIO_res ret;
	ret.beg = is_dynamic ? NULL : *resultp;
	ret.pos = ret.beg;
	ret.end = is_dynamic ? NULL : (*resultp + *lengthp);
	ret.is_dynamic = is_dynamic;
	*t = ret;
	return t;
}

/// End the object in case of success.
size_t _yIO_res_end(_yIO_res *t, char **resultp, size_t *lengthp) {
	assert(resultp != NULL);
	if (t->is_dynamic) {
		*resultp = t->beg;
	} else {
		assert(*resultp == t->beg);
	}
	const size_t used = _yIO_res_used(t);
	*lengthp = used;
	assert(*resultp + used == t->pos);
#ifndef NDEBUG
	t->beg = t->pos = t->end = NULL;
#endif
	return used;
}

/// End the object in case of error.
void _yIO_res_end_err(_yIO_res *t) {
	if (t->is_dynamic) {
		free(t->beg);
	}
#ifndef NDEBUG
	t->beg = t->pos = t->end = NULL;
#endif
}

int _yIO_res_reserve(_yIO_res *t, size_t newsize) {
	const size_t pos = _yIO_res_used(t);
	const size_t size = _yIO_res_size(t);
	assert(newsize > size);
	void * const p = realloc(t->is_dynamic ? t->beg : NULL, newsize * sizeof(*t->beg));
	if (p == NULL) {
		// NOTE! in case of allocation error
		// all resources are freed
		_yIO_res_end_err(t);
		return YIO_ERROR_ENOMEM;
	}
	if (!t->is_dynamic) {
		t->is_dynamic = true;
		memcpy(p, t->beg, size);
	}
	t->beg = p;
	t->pos = t->beg + pos;
	t->end = t->beg + newsize;
	return 0;
}

static inline
int _yIO_res_allocate_more(_yIO_res *t) {
	const size_t size = _yIO_res_size(t);
	const size_t _yIO_res_init_chunk = 32;
	assert(size < SIZE_MAX / 52);
	// golden ratio
	const size_t newsize = size ? size * 52 / 32 : _yIO_res_init_chunk;
	return _yIO_res_reserve(t, newsize);
}

int _yIO_res_putc(_yIO_res *t, char c) {
	assert(t->beg <= t->pos && t->pos <= t->end);
	if (t->pos == t->end) {
		const int err = _yIO_res_allocate_more(t);
		if (err) return err;
	}
	*t->pos++ = c;
	return 0;
}

int _yIO_res_putsn(_yIO_res *t, const char *ptr, size_t size) {
	while (_yIO_res_free_size(t) < size) {
		const int err = _yIO_res_allocate_more(t);
		if (err) return err;
	}
	memcpy(t->pos, ptr, size * sizeof(*t->pos));
	t->pos += size;
	return 0;
}

static
int _yIO_res_yprintf_cb(void *ptr, const char *data, size_t count) {
	_yIO_res *o = ptr;
	int err = 0;
	while (count--) {
		err = _yIO_res_putc(o, *data++);
		if (err) return err;
	}
	return 0;
}

#undef _yIO_res_yprintf
int _yIO_res_yprintf(_yIO_res *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yvbprintf(_yIO_res_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) {
		_yIO_res_end_err(t);
		return err;
	}
	return 0;
}

bool _yIO_res_remove_trailing_zeros_and_comma(_yIO_res *t) {
	bool fractional_part_removed = false;
	// remove trailing zeros
	char * const dest = t->pos;
	// god help me
	char *p = dest - 1;
	// there is dot, so the following loop will always stop
	while (*p == '0' && p != t->beg) {
		--p;
	}
	assert(isdigit((unsigned char)*p) || *p == '.' || strchr("abcdefABCDEF", *p));
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	t->pos = p;
	assert(t->pos >= t->beg);
	assert(t->end >= t->pos);
	return fractional_part_removed;
}
