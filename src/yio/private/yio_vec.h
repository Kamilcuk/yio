/**
 * @file
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Small output dynamic vector library implementation.
 * This library solely exists for yio_float_strfrom_stupid.c
 */
#pragma once
#include "private.h"
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#define _yIO_chunk  32

typedef struct _yIO_vec {
	char *beg;
	char *pos;
	char *end;
} _yIO_vec;

static inline
void _yIO_vec_init(_yIO_vec *t) {
	const _yIO_vec ret = {NULL, NULL, NULL};
	*t = ret;
}

static inline
void _yIO_vec_fini(_yIO_vec *t) {
	free(t->beg);
	t->beg = NULL;
}

static inline
char *_yIO_vec_data(_yIO_vec *t) {
	return t->beg;
}

static inline
int _yIO_vec_allocate_more(_yIO_vec *t) {
	const size_t pos = t->pos - t->beg;
	const size_t size = t->end - t->beg;
	const size_t newsize = size + _yIO_chunk;
	void *p = realloc(t->beg, newsize * sizeof(*t->beg));
	if (p == NULL) {
		// NOTE! in case of allocation error
		// all resources are freed
		_yIO_vec_fini(t);
		return YIO_ERROR_ENOMEM;
	}
	t->beg = p;
	t->pos = t->beg + pos;
	t->end = t->beg + newsize;
	return 0;
}

static inline
int _yIO_vec_putc(_yIO_vec *t, char c) {
	if (t->pos >= t->end) {
		const int err = _yIO_vec_allocate_more(t);
		if (err) return err;
	}
	*t->pos++ = c;
	return 0;
}

static inline
int _yIO_vec_puts(_yIO_vec *t, const char *s) {
	for (; *s != '\0'; ++s) {
		const int err = _yIO_vec_putc(t, *s);
		if (err) return err;
	}
	return 0;
}

static inline
size_t _yIO_vec_size(const _yIO_vec *t) {
	return t->pos - t->beg;
}

static
int _yIO_vec_yprintf_cb(void *ptr, const char *data, size_t count) {
	_yIO_vec *o = ptr;
	int err = 0;
	while (count--) {
		err = _yIO_vec_putc(o, *data++);
		if (err) return err;
	}
	return 0;
}

static inline
int _yIO_vec_yprintf(_yIO_vec *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int err = yvbprintf(_yIO_vec_yprintf_cb, t, data, fmt, &va);
	va_end(va);
	if (err < 0) {
		_yIO_vec_fini(t);
		return err;
	}
	return 0;
}

#define _yIO_vec_yprintf(t, ...)  _yIO_vec_yprintf(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Removes trailing zeros. There _has to_ be a dot in the string.
 * @param b
 * @return Did we remove the dot too?
 */
static inline
bool _yIO_vec_remove_trailing_zeros(_yIO_vec *t) {
	bool fractional_part_removed = false;
	// remove trailing zeros
	char * const dest = t->pos;
	// god help me
	char *p = dest - 1;
	// there is dot, so the following loop will always stop
	while (*p == '0' && p != t->beg) {
		--p;
	}
	assert(isdigit((unsigned char)*p) || *p == '.');
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	*p = '\0';
	t->pos = p;
	assert(t->pos >= t->beg);
	assert(t->end >= t->pos);
	return fractional_part_removed;
}

