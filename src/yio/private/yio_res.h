/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_PRIVATE_YIO_RES_H_
#define _yIO_YIO_PRIVATE_YIO_RES_H_
#include "../yio_config.h"
#include "../yio/io.h"
#include "../yio_error.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct _yIO_res {
	char *beg;
	char *pos;
	char *end;
	bool is_dynamic;
} _yIO_res;

/**
 * Initialized the res object.
 * Note that pointers are not needed here - only on the end.
 * @param t A pointer to _yIO_res to initialize.
 * @param resultp The statically allocated buffer or @c NULL.
 * @param lengthp The length of statically allocated buffer or 0.
 * @return Returns @c t.
 */
_yIO_rnn _yIO_access_w(1)
_yIO_res *_yIO_res_init(_yIO_res *t, char *resultp, size_t lengthp);

/// End the object in case of error.
static inline _yIO_access_rw(1)
void _yIO_res_end(_yIO_res *t) {
	if (t->is_dynamic) {
		free(t->beg);
	}
}

/// @see _yIO_begin
static inline _yIO_wur _yIO_nn()
char *_yIO_res_data(const _yIO_res *t) {
	return t->beg;
}

/// Return a poitner to beginning of memory
static inline _yIO_wur _yIO_nn()
char *_yIO_res_begin(const _yIO_res *t) {
	return t->beg;
}

/// Return the size of the container
static inline _yIO_wur _yIO_nn()
size_t _yIO_res_size(const _yIO_res *t) {
	return t->end - t->beg;
}

/// Return the used memory
static inline _yIO_wur _yIO_nn()
size_t _yIO_res_used(const _yIO_res *t) {
	return t->pos - t->beg;
}

/// Return the left free memory size
static inline _yIO_wur _yIO_nn()
size_t _yIO_res_free_size(const _yIO_res *t) {
	return t->end - t->pos;
}

/// Set the count of used bytes in container.
static inline _yIO_nn()
void _yIO_res_set_used(_yIO_res *t, size_t newused) {
	assert(newused < _yIO_res_size(t));
	t->pos = t->beg + newused;
}

/**
 * Allocate that much memory.
 * Note: @c newsize has to be greater then current size.
 * Shrinking the size is not supported.
 * @param t
 * @param newsize
 * @return
 */
static inline _yIO_wur _yIO_nn()
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

/**
 * Allocate golden ratio more memory.
 * @see _yIO_res_reserve
 */
static inline _yIO_wur _yIO_nn()
int _yIO_res_reserve_more(_yIO_res *t) {
	const size_t size = _yIO_res_size(t);
	const size_t _yIO_res_init_chunk = 32;
	assert(size < SIZE_MAX / 52);
	// golden ratio
	const size_t newsizecalc = size * 52 / 32;
	const size_t newsize = newsizecalc > _yIO_res_init_chunk ? newsizecalc : _yIO_res_init_chunk;
	return _yIO_res_reserve(t, newsize);
}
/// Add a character
static inline
int _yIO_res_putc(_yIO_res *t, char c) {
	assert(t->beg <= t->pos && t->pos <= t->end);
	if (t->pos == t->end) {
		const int err = _yIO_res_reserve_more(t);
		if (err) return err;
	}
	*t->pos++ = c;
	return 0;
}

/// Add memory
_yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2, 3)
int _yIO_res_putsn(_yIO_res *t, const char *ptr, size_t size);

/// Add a string
static inline _yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2)
int _yIO_res_puts(_yIO_res *t, const char *str) {
	return _yIO_res_putsn(t, str, strlen(str));
}

/// Print into the container
_yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2) _yIO_access_r(3)
int _yIO_res_yprintf_in(_yIO_res *t, yio_printdata_t *data, const char *fmt, ...);

/// Print into the container
#define _yIO_res_yprintf(t, ...)  _yIO_res_yprintf_in(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Removes trailing zeros. There _has to_ be a dot in the string.
 * @param b
 * @return Did we remove the dot too?
 */
bool _yIO_res_remove_trailing_zeros_and_comma(_yIO_res *t);

/**
 * Reserves at least @c newsize count of bytes and sets
 * the used count of bytes to newused.
 * @param t
 * @param newsize
 * @param newused
 * @return same as @c _yIO_res_reserve
 */
static inline _yIO_wur _yIO_nn()
int _yIO_res_resize2(_yIO_res *t, size_t newsize, size_t newused) {
	assert(newused <= newsize);
	const int err = _yIO_res_reserve(t, newsize);
	if (err) return err;
	_yIO_res_set_used(t, newused);
	return 0;
}

#ifndef YIO_CACHE_STACK_SIZE
#error "YIO_CACHE_STACK_SIZE not defined"
#define YIO_CACHE_STACK_SIZE 32
#endif

/**
 * @def _yIO_RES_AUTO_DECL(var)
 * @param var Name of the variable.
 * @brief Create a _yIO_res variable with YIO_CACHE_STACK_SIZE size bytes allocated on stack.
 */
#if YIO_CACHE_STACK_SIZE > 0
#define _yIO_RES_CONCAT(a, b)   a##b
#define _yIO_RES_XCONCAT(a, b)  _yIO_RES_CONCAT(a, b)
#define _yIO_RES_AUTO_DECL(var)  \
		char _yIO_RES_CONCAT(_buf_##var, __LINE__)[YIO_CACHE_STACK_SIZE]; \
		_yIO_res var; \
		_yIO_res_init(&var, _yIO_RES_CONCAT(_buf_##var, __LINE__), YIO_CACHE_STACK_SIZE)
#else
#define _yIO_RES_AUTO_DECL(var)  \
		_yIO_res var; \
		_yIO_res_init(&var, NULL, 0)
#endif

#endif /* _yIO_YIO_PRIVATE_YIO_RES_H_ */
