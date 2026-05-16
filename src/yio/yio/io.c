/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <limits.h>

#ifndef YIO_HAS_FILE
#error YIO_HAS_FILE is not defined
#endif

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* YIO_yio_*printf ------------------------------------------------------------------------ */

int YIO_yio_bprint(YIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vbprintf(out, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

int YIO_yio_print(const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vprintf(data, fmt, &va);
	va_end(va);
	return ret;
}

int YIO_yio_snprint(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vsprintf(dest, size, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

struct YIO_yio_vsprintf_ctx_s {
	char *dest;
	size_t size;
};

static
int YIO_yio_vsprintf_cb(void *arg, const char *ptr, size_t size) {
	struct YIO_yio_vsprintf_ctx_s *c = arg;
	if (c->size == 0) {
		return 0;
	}
	const bool not_enough_space = c->size < size + 1;
	size = not_enough_space ? c->size - 1 : size;
	memcpy(c->dest, ptr, size * sizeof(*c->dest));
	c->dest += size;
	c->size -= size;
	return not_enough_space ? YIO_ERROR_ENOBUFS : 0;
}

/* yio_v*printf except yio_vbprintf ------------------------------------------------------ */

static int sdcc_putchar_cb(void *arg, const char *data, size_t count) {
  (void)arg;
  for (size_t i = 0; i < count; ++i) {
    putchar(data[i]);
  }
  return 0;
}

int yio_vprintf(const yio_printdata_t *data, const char *fmt, va_list *va) {
#if YIO_OUTPUT_BACKEND_PUTCHAR
	return yio_vbprintf(sdcc_putchar_cb, NULL, data, fmt, va);
#elif YIO_OUTPUT_BACKEND_FD
	return yio_vdprint(1, data, fmt, va);
#elif YIO_HAS_FILE
	return yio_vfprintf(stdout, data, fmt, va);
#else
	(void)data; (void)fmt; (void)va;
	return YIO_ERROR_EIO;
#endif
}

int yio_vsprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, va_list *va) { // NOLINT(readability-non-const-parameter)
	struct YIO_yio_vsprintf_ctx_s ctx;
	ctx.dest = dest;
	ctx.size = size;
	const int ret = yio_vbprintf(YIO_yio_vsprintf_cb, &ctx, data, fmt, va);
	if (size > 0) {
		ctx.dest[0] = '\0';
	}
	return ret;
}

/* ------------------------------------------------------------------------- */

#if YIO_ENABLE_MALLOC

int YIO_yio_asprint(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vasprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

int YIO_yio_append(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vappend(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

struct YIO_yio_vappend_ctx_s {
	char *str;
	size_t size;
	size_t capacity;
};

static
int YIO_yio_vappend_cb(void *arg, const char *ptr, size_t size) {
	struct YIO_yio_vappend_ctx_s *p = arg;
	const size_t count = p->size + size + 1;
	assert(count < SIZE_MAX / sizeof(*p->str));
	if (count > p->capacity) {
		size_t new_cap = YIO_GOLDEN_INCREASE(p->capacity);
		if (new_cap < YIO_INIT_CAPACITY) new_cap = YIO_INIT_CAPACITY;
		if (new_cap < count) new_cap = count;
		void * const pnt = realloc(p->str, sizeof(*p->str) * new_cap);
		if (pnt == NULL) {
			free(p->str);
			p->str = NULL;
			p->size = 0;
			p->capacity = 0;
			return YIO_ERROR_ENOMEM;
		}
		p->str = pnt;
		p->capacity = new_cap;
	}
	memcpy(p->str + p->size, ptr, size * sizeof(*p->str));
	assert(p->size < SIZE_MAX - size);
	p->size += size;
	return 0;
}

int yio_vasprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	struct YIO_yio_vappend_ctx_s ctx;
	ctx.str = *strp;
	ctx.size = 0;
	ctx.capacity = (*strp != NULL) ? strlen(*strp) : 0;
	const int ret =  yio_vbprintf(YIO_yio_vappend_cb, &ctx, data, fmt, va);
	if (ret < 0) {
		free(ctx.str);
		ctx.str = NULL;
	}
	if (ctx.str != NULL) {
		ctx.str[ctx.size] = '\0';
		assert(ctx.size < SIZE_MAX);
		void * const pnt = realloc(ctx.str, sizeof(*ctx.str) * (ctx.size + 1));
		if (pnt != NULL) {
			ctx.str = pnt;
		}
	}
	*strp = ctx.str;
	return ret;
}

int yio_vappend(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	struct YIO_yio_vappend_ctx_s ctx;
	ctx.str = *strp;
	ctx.size = (*strp != NULL) ? strlen(*strp) : 0;
	ctx.capacity = ctx.size;
	const int ret =  yio_vbprintf(YIO_yio_vappend_cb, &ctx, data, fmt, va);
	if (ret < 0) {
		free(ctx.str);
		ctx.str = NULL;
	}
	if (ctx.str != NULL) {
		ctx.str[ctx.size] = '\0';
		assert(ctx.size < SIZE_MAX);
		void * const pnt = realloc(ctx.str, sizeof(*ctx.str) * (ctx.size + 1));
		if (pnt != NULL) {
			ctx.str = pnt;
		}
	}
	*strp = ctx.str;
	return ret;
}

#endif // YIO_ENABLE_MALLOC
