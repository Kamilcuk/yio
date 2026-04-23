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

/* YYIO_y*printf ------------------------------------------------------------------------ */

int YYIO_ybprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvbprintf(out, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yprintf(const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvprintf(data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_ysprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvsprintf(dest, size, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yaprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yreaprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvreaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

char *YYIO_yformatf(const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char * const ret = yvformatf(data, fmt, &va);
	va_end(va);
	return ret;
}

char *YYIO_yreformatf(char *str, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char * const ret = yvreformatf(str, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

struct YYIO_yvsprintf_ctx_s {
	char *dest;
	size_t size;
};

static
int YYIO_yvsprintf_cb(void *arg, const char *ptr, size_t size) {
	struct YYIO_yvsprintf_ctx_s *c = arg;
	if (c->size == 0) {
		return 0;
	}
	const bool not_enough_space = c->size < size + 1;
	size = not_enough_space ? c->size - 1 : size;
	memcpy(c->dest, ptr, size * sizeof(*c->dest));
	c->dest += size;
	return not_enough_space ? YIO_ERROR_ENOBUFS : 0;
}

struct YYIO_yreaprintf_ctx_s {
	char *str;
	size_t size;
};

static
int YYIO_yreaprintf_cb(void *arg, const char *ptr, size_t size) {
	struct YYIO_yreaprintf_ctx_s *p = arg;
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

int yvprintf(const yio_printdata_t *data, const char *fmt, va_list *va) {
#ifdef YIO_USE_OUTPUT_FD
	return yvdprintf(1, data, fmt, va);
#else
	return yvfprintf(stdout, data, fmt, va);
#endif
}

int yvsprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, va_list *va) { // NOLINT(readability-non-const-parameter)
	struct YYIO_yvsprintf_ctx_s ctx = {
			.dest = dest,
			.size = size,
	};
	const int ret = yvbprintf(YYIO_yvsprintf_cb, &ctx, data, fmt, va);
	if (size > 0) {
		ctx.dest[0] = '\0';
	}
	return ret;
}

int yvaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	*strp = NULL;
	return yvreaprintf(strp, data, fmt, va);
}

int yvreaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	struct YYIO_yreaprintf_ctx_s ctx = {
			.str = *strp,
			.size = (*strp != NULL) ? strlen(*strp) : 0,
	};
	const int ret =  yvbprintf(YYIO_yreaprintf_cb, &ctx, data, fmt, va);
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

char *yvformatf(const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yvreformatf(NULL, data, fmt, va);
}

char *yvreformatf(char *str, const yio_printdata_t *data, const char *fmt, va_list *va) {
	const int ret = yvreaprintf(&str, data, fmt, va);
	if (ret < 0) {
		return NULL;
	}
	return str;
}

