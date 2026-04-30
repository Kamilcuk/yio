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

#ifndef YIO_HAS_MALLOC
#error YIO_HAS_MALLOC is not defined
#endif

#define YYIO_MAX(a, b)  ((a) > (b) ? (a) : (b))
/// Rounds up 's' to the nearest multiple of 'a'. Works for any 'a' > 0.
#define YYIO_ALIGN_UP(s, a) (((s) + (a) - 1) / (a) * (a))

/// Round up SSO buffer to size_t to convert potential compiler padding
/// into usable capacity for the string.
#define YYIO_SSO_MIN_SIZE    YYIO_MAX(YIO_CACHE_STACK_SIZE, sizeof(size_t) * 2)
#define YYIO_SSO_SIZE        YYIO_ALIGN_UP(YYIO_SSO_MIN_SIZE, sizeof(size_t))

/// Represents a string with SSO and dynamic allocation.
typedef struct YYIO_string {
	/// if YIO_HAS_MALLOC:
	///  Bit 0: dynamic_flag (1=Heap, 0=SSO)
	///  Heap: Bits 1-63: Capacity (Heap)
	///  SSO: Bits 1-63: SSO_len
	///else:
	///  All bits is SSO_len.
	size_t info;
	union {
		#if YIO_HAS_MALLOC
		struct {
			size_t len;  /* Used only in Heap mode */
			char *ptr;   /* Used only in Heap mode */
		} h;
		#endif
		struct {
			char buf[YYIO_SSO_SIZE];
		} s;
	};
} YYIO_string;

/// Initializes the string object.
YYIO_access_w(1) static inline void YYIO_string_init(YYIO_string *t) {
	t->info = 0;
}

YYIO_wur static inline bool YYIO_string_is_dynamic(const YYIO_string *t) {
	#if YIO_HAS_MALLOC
	return t->info & 1;
	#else
	(void)t;
	return false;
	#endif
}

YYIO_wur static inline size_t YYIO_string_len(const YYIO_string *t) {
	#if YIO_HAS_MALLOC
	return YYIO_string_is_dynamic(t) ? t->h.len : (t->info >> 1);
	#else
	return t->info;
	#endif
}

YYIO_wur static inline char *YYIO_string_data(YYIO_string *t) {
	#if YIO_HAS_MALLOC
	return YYIO_string_is_dynamic(t) ? t->h.ptr : t->s.buf;
	#else
	return t->s.buf;
	#endif
}

YYIO_wur static inline size_t YYIO_string_capacity(const YYIO_string *t) {
	#if YIO_HAS_MALLOC
	return YYIO_string_is_dynamic(t) ? (t->info >> 1) : sizeof(t->s.buf);
	#else
	return sizeof(t->s.buf);
	#endif
}

/// Free the string object, freeing any dynamic memory.
static inline YYIO_access_rw(1)
void YYIO_string_free(YYIO_string *t) {
#if YIO_HAS_MALLOC
	if (YYIO_string_is_dynamic(t)) {
		free(t->h.ptr);
	}
#else
	(void)t;
#endif
}

/// Return the left free memory size
static inline YYIO_wur YYIO_nn()
size_t YYIO_string_free_size(const YYIO_string *t) {
	return YYIO_string_capacity(t) - YYIO_string_len(t);
}

/// Set the count of used bytes in container.
static inline YYIO_nn()
void YYIO_string_set_used(YYIO_string *t, size_t newused) {
	assert(newused <= YYIO_string_capacity(t));
	#if YIO_HAS_MALLOC
	if (YYIO_string_is_dynamic(t)) {
		t->h.len = newused;
	} else {
		t->info = (newused << 1);
	}
	#else
	t->info = newused;
	#endif
}

/// Allocate that much memory.
/// Note: @c newsize has to be greater than current capacity.
YYIO_wur YYIO_nn() int YYIO_string_reserve(YYIO_string *t, size_t newsize);

/// Allocate more memory.
YYIO_wur YYIO_nn() int YYIO_string_reserve_more(YYIO_string *t);

/// Add a character
static inline int YYIO_string_putc(YYIO_string *t, char c) {
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

/// Print into the container
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2) YYIO_access_r(3)
int YYIO_string_yprintf_in(YYIO_string *t, const yio_printdata_t *data, const char *fmt, ...);

/// Print into the container
#define YYIO_string_yprintf(t, ...)  YYIO_string_yprintf_in(t, YIO_PRINT_ARGUMENTS(__VA_ARGS__))

#endif /* YYIO_YIO_PRIVATE_YIO_STRING_H_ */
