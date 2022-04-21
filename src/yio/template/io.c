/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* _yIO_y*printf ------------------------------------------------------------------------ */

int _yΩIO_ybprintf(_yΩIO_printcb_t *out, void *arg, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvbprintf(out, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yprintf(const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvprintf(data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_ysprintf(Ychar *dest, size_t size, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvsprintf(dest, size, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yreaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvreaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

Ychar *_yΩIO_yformatf(const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Ychar * const ret = yπvformatf(data, fmt, &va);
	va_end(va);
	return ret;
}

Ychar *_yΩIO_yreformatf(Ychar *str, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Ychar * const ret = yπvreformatf(str, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

struct _yΩIO_yvsprintf_ctx_s {
	Ychar *dest;
	size_t size;
};

static
int _yΩIO_yvsprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	struct _yΩIO_yvsprintf_ctx_s *c = arg;
	if (c->size == 0) {
		return 0;
	}
	const bool not_enough_space = c->size < size + 1;
	size = not_enough_space ? c->size - 1 : size;
	memcpy(c->dest, ptr, size * sizeof(*c->dest));
	c->dest += size;
	return not_enough_space ? YIO_ERROR_ENOBUFS : 0;
}

struct _yΩIO_yreaprintf_ctx_s {
	Ychar *str;
	size_t size;
};

static
int _yΩIO_yreaprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	struct _yΩIO_yreaprintf_ctx_s *p = arg;
	const size_t count = p->size + size + 1;
	assert(count < SIZE_MAX / sizeof(*p->str));

	void * const pnt = realloc(p->str, sizeof(*p->str) * count);
	if (pnt == NULL) {
		free(p->str);
		p->str = NULL;
		p->size = 0;
		return YIO_ERROR_ENOMEM;
	}
	p->str = pnt;

	memcpy(p->str + p->size, ptr, size * sizeof(*p->str));
	assert(p->size < SIZE_MAX - size);
	p->size += size;

	return 0;
}


/* yv*printf except yvbprintf ------------------------------------------------------ */

int yπvprintf(const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
#ifdef YIO_USE_OUTPUT_FD
	return yπvdprintf(1, data, fmt, va);
#else
	return yπvfprintf(stdout, data, fmt, va);
#endif
}

int yπvsprintf(Ychar *dest, size_t size, const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	struct _yΩIO_yvsprintf_ctx_s ctx = {
			.dest = dest,
			.size = size,
	};
	const int ret = yπvbprintf(_yΩIO_yvsprintf_cb, &ctx, data, fmt, va);
	if (size > 0) {
		ctx.dest[0] = Yc('\0');
	}
	return ret;
}

int yπvaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	*strp = NULL;
	return yπvreaprintf(strp, data, fmt, va);
}

int yπvreaprintf(Ychar **strp, const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	struct _yΩIO_yreaprintf_ctx_s ctx = {
			.str = *strp,
			.size = (*strp != NULL) ? Ystrlen(*strp) : 0,
	};
	const int ret =  yπvbprintf(_yΩIO_yreaprintf_cb, &ctx, data, fmt, va);
	if (ret < 0) {
		free(ctx.str);
		ctx.str = NULL;
	}
	*strp = ctx.str;
	if (ctx.str != NULL) {
		ctx.str[ctx.size] = '\0';
	}
	return ret;
}

Ychar *yπvformatf(const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	return yπvreformatf(NULL, data, fmt, va);
}

Ychar *yπvreformatf(Ychar *str, const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	const int ret = yπvreaprintf(&str, data, fmt, va);
	if (ret < 0) {
		return NULL;
	}
	return str;
}

