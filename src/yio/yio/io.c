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

/* YYIO_yio_*printf ------------------------------------------------------------------------ */

int YYIO_yio_bprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vbprintf(out, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yio_printf(const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vprintf(data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yio_sprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vsprintf(dest, size, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yio_aprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

int YYIO_yio_reaprintf(char **strp, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vreaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

char *YYIO_yio_formatf(const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char * const ret = yio_vformatf(data, fmt, &va);
	va_end(va);
	return ret;
}

char *YYIO_yio_reformatf(char *str, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char * const ret = yio_vreformatf(str, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

struct YYIO_yio_vsprintf_ctx_s {
	char *dest;
	size_t size;
};

static
int YYIO_yio_vsprintf_cb(void *arg, const char *ptr, size_t size) {
	struct YYIO_yio_vsprintf_ctx_s *c = arg;
	if (c->size == 0) {
		return 0;
	}
	const bool not_enough_space = c->size < size + 1;
	size = not_enough_space ? c->size - 1 : size;
	memcpy(c->dest, ptr, size * sizeof(*c->dest));
	c->dest += size;
	return not_enough_space ? YIO_ERROR_ENOBUFS : 0;
}

struct YYIO_yio_vreaprintf_ctx_s {
	char *str;
	size_t size;
};

static
int YYIO_yio_vreaprintf_cb(void *arg, const char *ptr, size_t size) {
	struct YYIO_yio_vreaprintf_ctx_s *p = arg;
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


/* yio_v*printf except yio_vbprintf ------------------------------------------------------ */

int yio_vprintf(const yio_printdata_t *data, const char *fmt, va_list *va) {
#ifdef YIO_USE_OUTPUT_FD
	return yio_vdprintf(1, data, fmt, va);
#else
	return yio_vfprintf(stdout, data, fmt, va);
#endif
}

int yio_vsprintf(char *dest, size_t size, const yio_printdata_t *data, const char *fmt, va_list *va) { // NOLINT(readability-non-const-parameter)
	struct YYIO_yio_vsprintf_ctx_s ctx = {
			.dest = dest,
			.size = size,
	};
	const int ret = yio_vbprintf(YYIO_yio_vsprintf_cb, &ctx, data, fmt, va);
	if (size > 0) {
		ctx.dest[0] = '\0';
	}
	return ret;
}

int yio_vaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	*strp = NULL;
	return yio_vreaprintf(strp, data, fmt, va);
}

int yio_vreaprintf(char **strp, const yio_printdata_t *data, const char *fmt, va_list *va) {
	struct YYIO_yio_vreaprintf_ctx_s ctx = {
			.str = *strp,
			.size = (*strp != NULL) ? strlen(*strp) : 0,
	};
	const int ret =  yio_vbprintf(YYIO_yio_vreaprintf_cb, &ctx, data, fmt, va);
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

char *yio_vformatf(const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yio_vreformatf(NULL, data, fmt, va);
}

char *yio_vreformatf(char *str, const yio_printdata_t *data, const char *fmt, va_list *va) {
	const int ret = yio_vreaprintf(&str, data, fmt, va);
	if (ret < 0) {
		return NULL;
	}
	return str;
}

