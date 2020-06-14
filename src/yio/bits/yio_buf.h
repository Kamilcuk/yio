/**
 * @file yio_buf.h
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Small output buffer library implementation
 * This can run in two modes:
 * 1. When initialized with null, it measures length of bytes that would
 * have been written to the buffer without a terminating zero character.
 * 2. When initialized with not null, actually writes the bytes.
 */
#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef struct _yIO_buf {
	char *dest;
	size_t len;
} _yIO_buf;

static inline
_yIO_buf _yIO_buf_init(char *dest) {
	return (_yIO_buf){dest, 0};
}

static inline
size_t _yIO_buf_end(_yIO_buf *t) {
	if (t->dest != NULL) {
		*t->dest = '\0';
	}
	return t->len;
}

static inline
void _yIO_buf_putc(_yIO_buf *t, char c) {
	t->len++;
	if (t->dest != NULL) {
		*(t->dest++) = c;
	}
}

static inline
void _yIO_buf_puts(_yIO_buf *t, const char *s) {
	for (; *s != '\0'; ++s) {
		_yIO_buf_putc(t, *s);
	}
}

static inline
bool _yIO_buf_remove_trailing_zeros(_yIO_buf *b) {
	bool fractional_part_removed = false;
	if (b->dest == NULL) {
		return false;
	}
	// remove trailing zeros
	char * const dest = b->dest;
	const size_t len = b->len;
	// god help me
	char *p = dest - 1;
	// there is dot, so the following loop will always stop
	while (*p == '0') {
		--p;
	}
	assert(isdigit((unsigned char)*p) || *p == '.');
	if (*p != '.') {
		++p;
	} else {
		fractional_part_removed = true;
	}
	assert(dest - len < p && p <= dest);
	*p = '\0';
	return fractional_part_removed;
}

