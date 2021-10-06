/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 *
 * Functions returning a string result take a <tt>(resultp, lengthp)</tt> argument pair.
 * @c resultp has to be not NULL. If @c *resultp is not NULL and @c lengthp is
 * not NULL and the result fits into @c *lengthp units, then the result is stored
 * into @c *resultp. If @c *resultp is NULL, then the result is dynamically
 * allocated and stored into @c *resultp. In any case, if @c lengthp is not NULL,
 * then length of the string is stored in @c *lengthp.
 * The caller is expected to check if @c *resultp equal to the original buffer. If it
 * does, no action has to be taken. If it does not, that means that the result was
 * dynamically allocated and has to be @c freed.
 * In case of error, the content of @c *resultp and @c *lengthp are indeterminate.
 * Typical usage example:
 *
 *     char buf[200];
 *     char *result = buf; size_t length = sizeof(buf);
 *     err = some_function(, ..., &result, &length);
 *     _yIO_res_free_result(buf, result);
 *
 * The API here is written in a "self-cleaning" fashion - if any of the functions
 * return with error, then there is no need for cleanup, it is called automatically.
 */
m4_syncline(1)m4_dnl;
#pragma once
#include "../yio_common.h"
#include "../yio.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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
 * @param resultp A valie pointer to the result, see file comment above.
 * @param lengthp NULL or a pointer to size, see file comment above.
 * @return Returns @c t.
 */
_yIO_rnn _yIO_nn(1, 2) _yIO_access_w(1) _yIO_access_r(2) _yIO_access_r(3)
_yIO_res *_yIO_res_init(_yIO_res *t, char **resultp, size_t *lengthp);

#define _yIO_RES_INIT_ON_STACK(resultp, lengthp, size) \
		_yIO_res_init((_yIO_res){0}, (*resultp = (char[size]){0}, resultp), (*lengthp = size, lengthp))

/**
 * End the object in case of success.
 * Must be passed the same arguments as to _yIO_res_init!
 * @param t Same as _yIO_res_init.
 * @param resultp Same as _yIO_res_init.
 * @param lengthp Same as _yIO_res_init.
 * @return The count of characters written to *resultp.
 */
_yIO_nn(1, 2) _yIO_access_rw(1) _yIO_access_w(2) _yIO_access_w(3)
size_t _yIO_res_end(_yIO_res *t, char **resultp, size_t *lengthp);

/// End the object in case of error.
_yIO_access_rw(1)
void _yIO_res_end_err(_yIO_res *t);

/// Add a character
int _yIO_res_putc(_yIO_res *t, char c);

/// Add memory
_yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2, 3)
int _yIO_res_putsn(_yIO_res *t, const char *ptr, size_t size);

/// Add a string
static inline _yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2)
int _yIO_res_puts(_yIO_res *t, const char *str) {
	return _yIO_res_putsn(t, str, strlen(str));
}

/**
 * Allocate that much memory.
 * Note: @c newsize has to be greater then current size.
 * Shrinking the size is not supported.
 * @param t
 * @param newsize
 * @return
 */
_yIO_wur _yIO_nn()
int _yIO_res_reserve(_yIO_res *t, size_t newsize);

/// Print into the container
_yIO_wur _yIO_nn() _yIO_access_rw(1) _yIO_access_r(2) _yIO_access_r(3)
int _yIO_res_yprintf(_yIO_res *t, yio_printdata_t *data, const char *fmt, ...);

/// Print into the container
#define _yIO_res_yprintf(t, ...)  _yIO_res_yprintf(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Removes trailing zeros. There _has to_ be a dot in the string.
 * @param b
 * @return Did we remove the dot too?
 */
bool _yIO_res_remove_trailing_zeros_and_comma(_yIO_res *t);

/// Free the result returned by _yIO_res
static inline _yIO_nn(2)
void _yIO_res_free_result(char *buffer, char *resultp) {
	if (buffer != resultp) {
		free(resultp);
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
void _yIO_set_used(_yIO_res *t, size_t newused) {
	assert(newused < _yIO_res_size(t));
	t->pos = t->beg + newused;
}

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
	_yIO_set_used(t, newused);
	return 0;
}

