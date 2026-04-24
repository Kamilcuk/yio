/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_PRIVATE_YIO_STRING_H_
#define YYIO_YIO_PRIVATE_YIO_STRING_H_
#include "../yio_config.h"
#include "../yio/io.h"
#include "../yio_error.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define YYIO_MAX(a, b)  ((a) > (b) ? (a) : (b))
#define YYIO_SSO_SIZE   YYIO_MAX((size_t)YIO_CACHE_STACK_SIZE, sizeof(size_t) * 2)

typedef struct YYIO_string {
	union {
		struct {
			/* Bit 0: dynamic_flag (1=Heap, 0=SSO)
			 * Bits 1-63: Capacity (Heap) */
			size_t info;
			size_t len;  /* Used only in Heap mode */
			char *ptr;   /* Used only in Heap mode */
		} h;
		struct {
			/* Bit 0: dynamic_flag (0=SSO)
			 * Bits 1-63: SSO_len */
			size_t info;
			char buf[YYIO_SSO_SIZE];
		} s;
	};
} YYIO_string;

/**
 * Initializes the string object.
 */
static inline YYIO_access_w(1)
void YYIO_string_init(YYIO_string *t) {
	t->h.info = 0;
}

static inline bool YYIO_string_is_dynamic(const YYIO_string *t) {
	return t->h.info & 1;
}

static inline size_t YYIO_string_len(const YYIO_string *t) {
	return YYIO_string_is_dynamic(t) ? t->h.len : (t->h.info >> 1);
}

static inline char *YYIO_string_data(const YYIO_string *t) {
	return YYIO_string_is_dynamic(t) ? t->h.ptr : (char *)t->s.buf;
}

static inline size_t YYIO_string_capacity(const YYIO_string *t) {
	return YYIO_string_is_dynamic(t) ? (t->h.info >> 1) : sizeof(t->s.buf);
}

/// End the string object, freeing any dynamic memory.
static inline YYIO_access_rw(1)
void YYIO_string_end(YYIO_string *t) {
	if (YYIO_string_is_dynamic(t)) {
		free(t->h.ptr);
	}
}

/// Return the size of the container (capacity)
static inline YYIO_wur YYIO_nn()
size_t YYIO_string_size(const YYIO_string *t) {
	return YYIO_string_capacity(t);
}

/// Return the used memory
static inline YYIO_wur YYIO_nn()
size_t YYIO_string_used(const YYIO_string *t) {
	return YYIO_string_len(t);
}

/// Return the left free memory size
static inline YYIO_wur YYIO_nn()
size_t YYIO_string_free_size(const YYIO_string *t) {
	return YYIO_string_capacity(t) - YYIO_string_len(t);
}

/**
 * Set the count of used bytes in container.
 */
static inline YYIO_nn()
void YYIO_string_set_used(YYIO_string *t, size_t newused) {
	assert(newused <= YYIO_string_capacity(t));
	if (YYIO_string_is_dynamic(t)) {
		t->h.len = newused;
	} else {
		t->s.info = (newused << 1);
	}
}

/**
 * Allocate that much memory.
 * Note: @c newsize has to be greater than current capacity.
 */
YYIO_wur YYIO_nn()
int YYIO_string_reserve(YYIO_string *t, size_t newsize);

/**
 * Allocate more memory.
 */
YYIO_wur YYIO_nn()
int YYIO_string_reserve_more(YYIO_string *t);

/**
 * Add a character
 */
static inline
int YYIO_string_putc(YYIO_string *t, char c) {
	if (YYIO_string_free_size(t) == 0) {
		const int err = YYIO_string_reserve_more(t);
		if (err) return err;
	}
	const size_t len = YYIO_string_len(t);
	YYIO_string_data(t)[len] = c;
	YYIO_string_set_used(t, len + 1);
	return 0;
}

/// Add memory
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2, 3)
int YYIO_string_putsn(YYIO_string *t, const char *ptr, size_t size);

/// Add a string
static inline YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2)
int YYIO_string_puts(YYIO_string *t, const char *str) {
	return YYIO_string_putsn(t, str, strlen(str));
}

/// Print into the container
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2) YYIO_access_r(3)
int YYIO_string_yprintf_in(YYIO_string *t, yio_printdata_t *data, const char *fmt, ...);

/// Print into the container
#define YYIO_string_yprintf(t, ...)  YYIO_string_yprintf_in(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

/**
 * Removes trailing zeros. There _has to_ be a dot in the string.
 * @param b
 * @return Did we remove the dot too?
 */
bool YYIO_string_remove_trailing_zeros_and_comma(YYIO_string *t);

#endif /* YYIO_YIO_PRIVATE_YIO_STRING_H_ */
