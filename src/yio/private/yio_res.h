/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_RES_H_
#define YYIO_YIO_PRIVATE_YIO_RES_H_
#include "../yio_config.h"
#include "../yio/io.h"
#include "../yio_error.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct YYIO_res {
	char *beg;
	char *pos;
	char *end;
	bool is_dynamic;
} YYIO_res;

/**
 * Initialized the res object.
 * Note that pointers are not needed here - only on the end.
 * @param t A pointer to YYIO_res to initialize.
 * @param resultp The statically allocated buffer or @c NULL.
 * @param lengthp The length of statically allocated buffer or 0.
 * @return Returns @c t.
 */
YYIO_rnn YYIO_access_w(1)
YYIO_res *YYIO_res_init(YYIO_res *t, char *resultp, size_t lengthp);

/// End the object in case of error.
static inline YYIO_access_rw(1)
void YYIO_res_end(YYIO_res *t) {
	if (t->is_dynamic) {
		free(t->beg);
	}
}

/// @see YYIO_begin
static inline YYIO_wur YYIO_nn()
char *YYIO_res_data(const YYIO_res *t) {
	return t->beg;
}

/// Return a poitner to beginning of memory
static inline YYIO_wur YYIO_nn()
char *YYIO_res_begin(const YYIO_res *t) {
	return t->beg;
}

/// Return the size of the container
static inline YYIO_wur YYIO_nn()
size_t YYIO_res_size(const YYIO_res *t) {
	return t->end - t->beg;
}

/// Return the used memory
static inline YYIO_wur YYIO_nn()
size_t YYIO_res_used(const YYIO_res *t) {
	return t->pos - t->beg;
}

/// Return the left free memory size
static inline YYIO_wur YYIO_nn()
size_t YYIO_res_free_size(const YYIO_res *t) {
	return t->end - t->pos;
}

/// Set the count of used bytes in container.
static inline YYIO_nn()
void YYIO_res_set_used(YYIO_res *t, size_t newused) {
	assert(newused < YYIO_res_size(t));
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
static inline YYIO_wur YYIO_nn()
int YYIO_res_reserve(YYIO_res *t, size_t newsize) {
	const size_t pos = YYIO_res_used(t);
	const size_t size = YYIO_res_size(t);
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
 * @see YYIO_res_reserve
 */
static inline YYIO_wur YYIO_nn()
int YYIO_res_reserve_more(YYIO_res *t) {
	const size_t size = YYIO_res_size(t);
	const size_t YYIO_res_init_chunk = 32;
	assert(size < SIZE_MAX / 52);
	// golden ratio
	const size_t newsizecalc = size * 52 / 32;
	const size_t newsize = newsizecalc > YYIO_res_init_chunk ? newsizecalc : YYIO_res_init_chunk;
	return YYIO_res_reserve(t, newsize);
}
/// Add a character
static inline
int YYIO_res_putc(YYIO_res *t, char c) {
	assert(t->beg <= t->pos && t->pos <= t->end);
	if (t->pos == t->end) {
		const int err = YYIO_res_reserve_more(t);
		if (err) return err;
	}
	*t->pos++ = c;
	return 0;
}

/// Add memory
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2, 3)
int YYIO_res_putsn(YYIO_res *t, const char *ptr, size_t size);

/// Add a string
static inline YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2)
int YYIO_res_puts(YYIO_res *t, const char *str) {
	return YYIO_res_putsn(t, str, strlen(str));
}

/// Print into the container
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2) YYIO_access_r(3)
int YYIO_res_yprintf_in(YYIO_res *t, yio_printdata_t *data, const char *fmt, ...);

/// Print into the container
#define YYIO_res_yprintf(t, ...)  YYIO_res_yprintf_in(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Removes trailing zeros. There _has to_ be a dot in the string.
 * @param b
 * @return Did we remove the dot too?
 */
bool YYIO_res_remove_trailing_zeros_and_comma(YYIO_res *t);

#ifndef YIO_CACHE_STACK_SIZE
#error "YIO_CACHE_STACK_SIZE not defined"
#define YIO_CACHE_STACK_SIZE 32
#endif

/**
 * @def YYIO_RES_AUTO_DECL(var)
 * @param var Name of the variable.
 * @brief Create a YYIO_res variable with YIO_CACHE_STACK_SIZE size bytes allocated on stack.
 */
#if YIO_CACHE_STACK_SIZE > 0
#define YYIO_RES_CONCAT(a, b)   a##b
#define YYIO_RES_XCONCAT(a, b)  YYIO_RES_CONCAT(a, b)
#define YYIO_RES_AUTO_DECL(var)  \
		char YYIO_RES_CONCAT(_buf_##var, __LINE__)[YIO_CACHE_STACK_SIZE]; \
		YYIO_res var; \
		YYIO_res_init(&(var), YYIO_RES_CONCAT(_buf_##var, __LINE__), YIO_CACHE_STACK_SIZE)
#else
#define YYIO_RES_AUTO_DECL(var)  \
		YYIO_res var; \
		YYIO_res_init(&(var), NULL, 0)
#endif

#endif /* YYIO_YIO_PRIVATE_YIO_RES_H_ */
