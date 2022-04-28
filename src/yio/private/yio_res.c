/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
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

_yIO_res *_yIO_res_init(_yIO_res *t, char *resultp, size_t lengthp) {
	const bool is_dynamic = resultp == NULL || lengthp == 0;
	_yIO_res ret;
	ret.beg = is_dynamic ? NULL : resultp;
	ret.pos = ret.beg;
	ret.end = is_dynamic ? NULL : (resultp + lengthp);
	ret.is_dynamic = is_dynamic;
	*t = ret;
	return t;
}

/// End the object in case of error.
void _yIO_res_end(_yIO_res *t) {
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
	if (newsize <= size) {
		return 0;
	}
	void *const p = realloc(t->is_dynamic ? t->beg : NULL, newsize * sizeof(*t->beg));
	if (p == NULL) {
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

int _yIO_res_reserve_more(_yIO_res *t) {
	const size_t size = _yIO_res_size(t);
	const size_t _yIO_res_init_chunk = 32;
	assert(size < SIZE_MAX / 52);
	// golden ratio
	const size_t newsizecalc = size * 52 / 32;
	const size_t newsize = newsizecalc > _yIO_res_init_chunk ? newsizecalc : _yIO_res_init_chunk;
	return _yIO_res_reserve(t, newsize);
}

int _yIO_res_putc(_yIO_res *t, char c) {
	assert(t->beg <= t->pos && t->pos <= t->end);
	if (t->pos == t->end) {
		const int err = _yIO_res_reserve_more(t);
		if (err) return err;
	}
	*t->pos++ = c;
	return 0;
}

int _yIO_res_putsn(_yIO_res *t, const char *ptr, size_t size) {
	while (_yIO_res_free_size(t) < size) {
		const int err = _yIO_res_reserve_more(t);
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

int _yIO_res_yprintf_in(_yIO_res *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yvbprintf(_yIO_res_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) {
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
