/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_PRIVATE_YIO_BUF_H_
#define YIO_YIO_PRIVATE_YIO_BUF_H_
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

#define YIO_MAX(a, b)  ((a) > (b) ? (a) : (b))
/// Rounds up 's' to the nearest multiple of 'a'. Works for any 'a' > 0.
#define YIO_ALIGN_UP(s, a) (((s) + (a) - 1) / (a) * (a))

#ifdef __SIZEOF_SIZE_T__
#define YIO_SIZEOF_SIZE_T  __SIZEOF_SIZE_T__
#else
#define YIO_SIZEOF_SIZE_T  SIZE_MAX / CHAR_BIT
#endif

#if YIO_ENABLE_MALLOC
#define YIO_SSO_MIN_SIZE    YIO_MAX(YIO_SSO_BUFFER_SIZE, YIO_SIZEOF_SIZE_T * 2)
#else
#define YIO_SSO_MIN_SIZE    YIO_SSO_BUFFER_SIZE
#endif

/// Round up SSO buffer to size_t to convert potential compiler padding
/// into usable capacity for the string.
#define YIO_SSO_SIZE        YIO_ALIGN_UP(YIO_SSO_MIN_SIZE, YIO_SIZEOF_SIZE_T)

typedef
#if YIO_ENABLE_MALLOC
	size_t
#elif YIO_SSO_SIZE <= UINT8_MAX
	uint8_t
#elif YIO_SSO_SIZE <= UINT16_MAX
	uint16_t
#elif YIO_SSO_SIZE <= UINT32_MAX
	uint32_t
#elif YIO_SSO_SIZE <= UINT64_MAX
	uint64_t
#else
#error too big YIO_SSO_SIZE
#endif
	YIO_buf_size_t;

/// Represents a string with SSO and dynamic allocation.
typedef struct YIO_buf {
	///  Bit 0: dynamic_flag (1=Heap, 0=SSO)
	///  Heap: Bits 1-63: Capacity (Heap)
	///  SSO: Bits 1-63: SSO_len
	YIO_buf_size_t info;
	union {
#if YIO_ENABLE_MALLOC
		struct {
			size_t len;  /* Used only in Heap mode */
			char * __sized_by(info) ptr;   /* Used only in Heap mode */
		} h;
#endif
		char buf[YIO_SSO_SIZE];
	};
} YIO_buf;

/// Initializes the string object.
YIO_access_w(1) static inline void YIO_buf_init(YIO_buf *t) {
	t->info = 0;
}

YIO_wur static inline bool YIO_buf_is_dynamic(const YIO_buf *t) {
	#if YIO_ENABLE_MALLOC
	return t->info & 1;
	#else
	(void)t;
	return false;
	#endif
}

YIO_wur static inline size_t YIO_buf_len(const YIO_buf *t) {
	#if YIO_ENABLE_MALLOC
	return YIO_buf_is_dynamic(t) ? t->h.len : (t->info >> 1);
	#else
	return t->info;
	#endif
}

YIO_wur static inline char * __indexable YIO_buf_data(YIO_buf *t) {
	#if YIO_ENABLE_MALLOC
	return YIO_buf_is_dynamic(t) ? t->h.ptr : t->buf;
	#else
	return t->buf;
	#endif
}

YIO_wur static inline size_t YIO_buf_capacity(const YIO_buf *t) {
	#if YIO_ENABLE_MALLOC
	return YIO_buf_is_dynamic(t) ? t->info : sizeof(t->buf);
	#else
	return sizeof(t->buf);
	#endif
}

/// Set the count of used bytes in container.
static inline YIO_nn()
void YIO_buf_set_used(YIO_buf *t, size_t newused) {
	assert(newused <= YIO_buf_capacity(t));
	#if YIO_ENABLE_MALLOC
	if (YIO_buf_is_dynamic(t)) {
		t->h.len = newused;
	} else {
		t->info = (newused << 1);
	}
	#else
	t->info = newused;
	#endif
}

/// Clears the string.
static inline void YIO_buf_clear(YIO_buf *t) {
	YIO_buf_set_used(t, 0);
}

/// Free the string object, freeing any dynamic memory.
static inline YIO_access_rw(1)
void YIO_buf_fini(YIO_buf *t) {
#if YIO_ENABLE_MALLOC
	if (YIO_buf_is_dynamic(t)) {
		free(t->h.ptr);
	}
#else
	(void)t;
#endif
}

/// Return the left free memory size
static inline YIO_wur YIO_nn()
size_t YIO_buf_free_size(const YIO_buf *t) {
	return YIO_buf_capacity(t) - YIO_buf_len(t);
}

#if YIO_ENABLE_MALLOC
/// Allocate that much memory.
/// Note: @c newsize has to be greater than current capacity.
YIO_wur YIO_nn() int YIO_buf_reserve(YIO_buf *t, size_t newsize);
/// Allocate more memory.
int YIO_buf_reserve_more(YIO_buf *t, size_t min_add);
#else
static int YIO_buf_reserve(YIO_buf *t, size_t newsize) {
	(void)t; (void)newsize; return YIO_ERROR_ENOMEM;
}
static int YIO_buf_reserve_more(YIO_buf *t, size_t min_add) {
	(void)t; (void)min_add; return YIO_ERROR_ENOMEM;
}
#endif

/// Add a character
static inline int YIO_buf_putc(YIO_buf *t, char c) {
	if (YIO_buf_free_size(t) == 0) {
		const int err = YIO_buf_reserve_more(t, 1);
		if (err) return err;
	}
	const size_t len = YIO_buf_len(t);
	YIO_buf_data(t)[len] = c;
	YIO_buf_set_used(t, len + 1);
	return 0;
}

/// Ensures the string is null-terminated and returns the pointer.
static inline char *YIO_buf_c_str(YIO_buf *t) {
	const size_t len = YIO_buf_len(t);
	if (len == 0 || YIO_buf_data(t)[len - 1] != '\0') {
		if (YIO_buf_putc(t, '\0') != 0) return NULL;
	}
	return YIO_buf_data(t);
}

/// Add memory
YIO_wur YIO_nn() YIO_access_rw(1) YIO_access_r(2, 3)
int YIO_buf_putsn(YIO_buf *t, const char * __sized_by(size) ptr, size_t size);

static int YIO_buf_yprintf_cb(void *ptr, const char * __sized_by(count) data, size_t count) {
	YIO_buf *o = (YIO_buf *)ptr;
	return YIO_buf_putsn(o, data, count);
}

int YIO_print_uint_in(yio_printctx_t *t, unsigned int arg, bool is_negative);
int YIO_print_ulong_in(yio_printctx_t *t, unsigned long arg, bool is_negative);
#if YIO_HAS_LLONG
int YIO_print_ullong_in(yio_printctx_t *t, unsigned long long arg, bool is_negative);
#endif
#if YIO_HAS_INT128
int YIO_print_uint128_in(yio_printctx_t *t, unsigned __int128 arg, bool is_negative);
#endif

static inline int YIO_buf_print_u_in(YIO_buf *o, struct yio_printfmt_s pf, unsigned int v) {
    yio_printctx_t ctx = {0};
  	ctx.pf = pf;
  	ctx.out = YIO_buf_yprintf_cb;
  	ctx.outarg = o;
    return YIO_print_uint_in(&ctx, v, false);
}
static inline int YIO_buf_print_ul_in(YIO_buf *o, struct yio_printfmt_s pf, unsigned long v) {
    yio_printctx_t ctx = {0};
  	ctx.pf = pf;
  	ctx.out = YIO_buf_yprintf_cb;
  	ctx.outarg = o;
    return YIO_print_ulong_in(&ctx, v, false);
}
#if YIO_HAS_LLONG
static inline int YIO_buf_print_ull_in(YIO_buf *o, struct yio_printfmt_s pf, unsigned long long v) {
    yio_printctx_t ctx = {0};
  	ctx.pf = pf;
  	ctx.out = YIO_buf_yprintf_cb;
  	ctx.outarg = o;
    return YIO_print_ullong_in(&ctx, v, false);
}
#endif
#if YIO_HAS_INT128
static inline int YIO_buf_print_u128_in(YIO_buf *o, struct yio_printfmt_s pf, unsigned __int128 v) {
    yio_printctx_t ctx = {0};
  	ctx.pf = pf;
  	ctx.out = YIO_buf_yprintf_cb;
  	ctx.outarg = o;
    return YIO_print_uint128_in(&ctx, v, false);
}
#endif

static inline int YIO_buf_print_int(YIO_buf *o, struct yio_printfmt_s pf, int val) {
  yio_printctx_t ctx = {0};
  ctx.pf = pf;
  ctx.out = YIO_buf_yprintf_cb;
  ctx.outarg = o;
	const bool is_neg = val < 0;
	const unsigned abs_val = is_neg ? -(unsigned)val : (unsigned)val;
	return YIO_print_uint_in(&ctx, abs_val, is_neg);
}

/**
 * Compare two YIO_buf objects for equality.
 * @param a First string.
 * @param b Second string.
 * @return true if strings have the same length and identical content, false otherwise.
 */
static inline bool YIO_buf_equal(const YIO_buf *a, const YIO_buf *b) {
    const size_t len_a = YIO_buf_len(a);
    const size_t len_b = YIO_buf_len(b);
    if (len_a != len_b) return false;
    if (len_a == 0) return true;
    /* We need to cast away const because YIO_buf_data currently takes a non-const pointer */
    return memcmp(YIO_buf_data((YIO_buf *)a), YIO_buf_data((YIO_buf *)b), len_a) == 0;
}

/**
 * Removes trailing zeros and the decimal point from the string.
 * Assumes that the string represents a floating point number and HAS a decimal point.
 * @param t The string to modify.
 * @return true if the decimal point was removed.
 */
static inline bool YIO_buf_remove_trailing_zeros_and_dot(YIO_buf *t) {
	bool dot_removed = false;
	const size_t len = YIO_buf_len(t);
	if (len == 0) return false;
	char * const data = YIO_buf_data(t);
	// Ensure there is a dot. If not, this function should not be called.
	assert(memchr(data, '.', len) != NULL);
	char *p = data + len - 1;
	while (p > data && *p == '0') {
		--p;
	}
	if (*p == '.') {
		dot_removed = true;
	} else {
		++p;
	}
	YIO_buf_set_used(t, (size_t)(p - data));
	return dot_removed;
}

#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_PRIVATE_YIO_BUF_H_
