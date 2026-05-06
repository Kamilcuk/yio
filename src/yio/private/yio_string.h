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
#ifdef __cplusplus
extern "C" {
#endif

#include "../yio_config.h"
#include "../yio/io.h"
#include "../yio_error.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifndef YIO_ENABLE_MALLOC
#error YIO_ENABLE_MALLOC is not defined
#endif

#define YYIO_MAX(a, b)  ((a) > (b) ? (a) : (b))
/// Rounds up 's' to the nearest multiple of 'a'. Works for any 'a' > 0.
#define YYIO_ALIGN_UP(s, a) (((s) + (a) - 1) / (a) * (a))

#ifdef __SIZEOF_SIZE_T__
#define YYIO_SIZEOF_SIZE_T  __SIZEOF_SIZE_T__
#else
#define YYIO_SIZEOF_SIZE_T  SIZE_MAX / CHAR_BIT
#endif

#if YIO_ENABLE_MALLOC
#define YYIO_SSO_MIN_SIZE    YYIO_MAX(YIO_SSO_BUFFER_SIZE, YYIO_SIZEOF_SIZE_T * 2)
#else
#define YYIO_SSO_MIN_SIZE    YIO_SSO_BUFFER_SIZE
#endif

/// Round up SSO buffer to size_t to convert potential compiler padding
/// into usable capacity for the string.
#define YYIO_SSO_SIZE        YYIO_ALIGN_UP(YYIO_SSO_MIN_SIZE, YYIO_SIZEOF_SIZE_T)

typedef
#if YIO_ENABLE_MALLOC
	size_t
#elif YYIO_SSO_SIZE <= UINT8_MAX
	uint8_t
#elif YYIO_SSO_SIZE <= UINT16_MAX
	uint16_t
#elif YYIO_SSO_SIZE <= UINT32_MAX
	uint32_t
#elif YYIO_SSO_SIZE <= UINT64_MAX
	uint64_t
#else
#error too big YYIO_SSO_SIZE
#endif
	YYIO_string_size_t;

/// Represents a string with SSO and dynamic allocation.
typedef struct YYIO_string {
	///  Bit 0: dynamic_flag (1=Heap, 0=SSO)
	///  Heap: Bits 1-63: Capacity (Heap)
	///  SSO: Bits 1-63: SSO_len
	YYIO_string_size_t info;
	union {
#if YIO_ENABLE_MALLOC
		struct {
			size_t len;  /* Used only in Heap mode */
			char * __sized_by(info >> 1) ptr;   /* Used only in Heap mode */
		} h;
#endif
		char buf[YYIO_SSO_SIZE];
	};
} YYIO_string;

/// Initializes the string object.
YYIO_access_w(1) static inline void YYIO_string_init(YYIO_string *t) {
	t->info = 0;
}

YYIO_wur static inline bool YYIO_string_is_dynamic(const YYIO_string *t) {
	#if YIO_ENABLE_MALLOC
	return t->info & 1;
	#else
	(void)t;
	return false;
	#endif
}

YYIO_wur static inline size_t YYIO_string_len(const YYIO_string *t) {
	#if YIO_ENABLE_MALLOC
	return YYIO_string_is_dynamic(t) ? t->h.len : (t->info >> 1);
	#else
	return t->info;
	#endif
}

YYIO_wur static inline char * __indexable YYIO_string_data(YYIO_string *t) {
	#if YIO_ENABLE_MALLOC
	return YYIO_string_is_dynamic(t) ? t->h.ptr : t->buf;
	#else
	return t->buf;
	#endif
}

YYIO_wur static inline size_t YYIO_string_capacity(const YYIO_string *t) {
	#if YIO_ENABLE_MALLOC
	return YYIO_string_is_dynamic(t) ? (t->info >> 1) : sizeof(t->buf);
	#else
	return sizeof(t->buf);
	#endif
}

/// Free the string object, freeing any dynamic memory.
static inline YYIO_access_rw(1)
void YYIO_string_free(YYIO_string *t) {
#if YIO_ENABLE_MALLOC
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
	#if YIO_ENABLE_MALLOC
	if (YYIO_string_is_dynamic(t)) {
		t->h.len = newused;
	} else {
		t->info = (newused << 1);
	}
	#else
	t->info = newused;
	#endif
}

#if YIO_ENABLE_MALLOC
/// Allocate that much memory.
/// Note: @c newsize has to be greater than current capacity.
YYIO_wur YYIO_nn() int YYIO_string_reserve(YYIO_string *t, size_t newsize);
/// Allocate more memory.
int YYIO_string_reserve_more(YYIO_string *t, size_t min_add);
#else
static int YYIO_string_reserve(YYIO_string *t, size_t newsize) {
	(void)t; (void)newsize; return YIO_ERROR_ENOMEM;
}
static int YYIO_string_reserve_more(YYIO_string *t, size_t min_add) {
	(void)t; (void)min_add; return YIO_ERROR_ENOMEM;
}
#endif

/// Add a character
static inline int YYIO_string_putc(YYIO_string *t, char c) {
	if (YYIO_string_free_size(t) == 0) {
		const int err = YYIO_string_reserve_more(t, 1);
		if (err) return err;
	}
	const size_t len = YYIO_string_len(t);
	YYIO_string_data(t)[len] = c;
	YYIO_string_set_used(t, len + 1);
	return 0;
}

/// Add memory
YYIO_wur YYIO_nn() YYIO_access_rw(1) YYIO_access_r(2, 3)
int YYIO_string_putsn(YYIO_string *t, const char * __sized_by(size) ptr, size_t size);

static int YYIO_string_yprintf_cb(void *ptr, const char * __sized_by(count) data, size_t count) {
	YYIO_string *o = ptr;
	return YYIO_string_putsn(o, data, count);
}

int YYIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YYIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);
#if YYIO_HAS_LLONG
int YYIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#endif
#if YYIO_HAS_INT128
int YYIO_print_uint128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#endif

static inline int YYIO_string_print_u_in(YYIO_string *o, struct yio_printfmt_s pf, unsigned int v) {
    yio_printctx_t ctx = {.pf = pf, .out = YYIO_string_yprintf_cb, .outarg = o};
    return YYIO_print_uint_in(&ctx, v, false);
}
static inline int YYIO_string_print_ul_in(YYIO_string *o, struct yio_printfmt_s pf, unsigned long v) {
    yio_printctx_t ctx = {.pf = pf, .out = YYIO_string_yprintf_cb, .outarg = o};
    return YYIO_print_ulong_in(&ctx, v, false);
}
#if YYIO_HAS_LLONG
static inline int YYIO_string_print_ull_in(YYIO_string *o, struct yio_printfmt_s pf, unsigned long long v) {
    yio_printctx_t ctx = {.pf = pf, .out = YYIO_string_yprintf_cb, .outarg = o};
    return YYIO_print_ullong_in(&ctx, v, false);
}
#endif
#if YYIO_HAS_INT128
static inline int YYIO_string_print_u128_in(YYIO_string *o, struct yio_printfmt_s pf, unsigned __int128 v) {
    yio_printctx_t ctx = {.pf = pf, .out = YYIO_string_yprintf_cb, .outarg = o};
    return YYIO_print_uint128_in(&ctx, v, false);
}
#endif

static inline int YYIO_string_print_int(YYIO_string *t, struct yio_printfmt_s fmt, int val) {
	yio_printctx_t ctx = {
		.pf = fmt,
		.out = YYIO_string_yprintf_cb,
		.outarg = t,
	};
	const bool is_neg = val < 0;
	const unsigned abs_val = is_neg ? -(unsigned)val : (unsigned)val;
	return YYIO_print_uint_in(&ctx, abs_val, is_neg);
}

/**
 * Compare two YYIO_string objects for equality.
 * @param a First string.
 * @param b Second string.
 * @return true if strings have the same length and identical content, false otherwise.
 */
static inline bool YYIO_string_equal(const YYIO_string *a, const YYIO_string *b) {
    const size_t len_a = YYIO_string_len(a);
    const size_t len_b = YYIO_string_len(b);
    if (len_a != len_b) return false;
    if (len_a == 0) return true;
    /* We need to cast away const because YYIO_string_data currently takes a non-const pointer */
    return memcmp(YYIO_string_data((YYIO_string *)a), YYIO_string_data((YYIO_string *)b), len_a) == 0;
}

/**
 * Removes trailing zeros and the decimal point from the string.
 * Assumes that the string represents a floating point number and HAS a decimal point.
 * @param t The string to modify.
 * @return true if the decimal point was removed.
 */
static inline bool YYIO_string_remove_trailing_zeros_and_dot(YYIO_string *t) {
	bool dot_removed = false;
	const size_t len = YYIO_string_len(t);
	if (len == 0) return false;
	char * const data = YYIO_string_data(t);
	// Ensure there is a dot. If not, this function should not be called.
	assert(strchr(data, '.') != NULL);
	char *p = data + len - 1;
	while (p > data && *p == '0') {
		--p;
	}
	if (*p == '.') {
		dot_removed = true;
	} else {
		++p;
	}
	YYIO_string_set_used(t, (size_t)(p - data));
	return dot_removed;
}

#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_PRIVATE_YIO_STRING_H_
