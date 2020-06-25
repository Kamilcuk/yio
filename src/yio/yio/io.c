/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config(yio);
#include "private.h"
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <strings.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* _yIO_y*printf ------------------------------------------------------------------------ */

int _yIO_ybprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvbprintf(out, arg, data, &va);
	va_end(va);
	return ret;
}

int _yIO_yprintf(const struct _yIO_printdata_s *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvprintf(data, &va);
	va_end(va);
	return ret;
}

int _yIO_yfprintf(FILE *file, const struct _yIO_printdata_s *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvfprintf(file, data, &va);
	va_end(va);
	return ret;
}

int _yIO_ysprintf(Ychar *dest, size_t size, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvsprintf(dest, size, data, &va);
	va_end(va);
	return ret;
}

int _yIO_yaprintf(Ychar **strp, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvaprintf(strp, data, &va);
	va_end(va);
	return ret;
}

int _yIO_yreaprintf(Ychar **strp, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvreaprintf(strp, data, &va);
	va_end(va);
	return ret;
}

Ychar *_yIO_yformatf(yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	char * const ret = yvformatf(data, &va);
	va_end(va);
	return ret;
}

Ychar *_yIO_yreformatf(Ychar *str, yio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	char * const ret = yvreformatf(str, data, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

static inline
size_t _yIO_fwrite(FILE* file, const Ychar* str, size_t size) {
#if YTYPE_YIO
	return fwrite(str, 1, size, file);
#elif YTYPE_YWIO
	for (size_t n = 0; n < size; n++) {
		if (fputwc(str[n], file) == YEOF) {
			return n;
		}
	}
	return size;
#endif
}

static
int _yIO_yvfprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = _yIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}

struct _yIO_yvsprintf_ctx_s {
	Ychar *dest;
	size_t size;
};

static
int _yIO_yvsprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	struct _yIO_yvsprintf_ctx_s *c = arg;
	if (c->size == 0) {
		return 0;
	}
	const bool not_enough_space = c->size < size + 1;
	size = not_enough_space ? c->size - 1 : size;
	memcpy(c->dest, ptr, size);
	c->dest += size;
	return not_enough_space ? YIO_ERROR_ENOBUFS : 0;
}

struct _yIO_yreaprintf_ctx_s {
	Ychar *str;
	size_t size;
};

static
int _yIO_yreaprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	struct _yIO_yreaprintf_ctx_s *p = arg;
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
	memcpy(p->str + p->size, ptr, size);
	assert(p->size < SIZE_MAX - size);
	p->size += size;
	return 0;
}


/* yv*printf except yvbprintf ------------------------------------------------------ */

int yvprintf(yio_printdata_t *data, va_list *va) {
#ifdef YIO_USE_OUTPUT_FD
	return yvdprintf(1, data, va);
#else
	return yvfprintf(stdout, data, va);
#endif
}

int yvfprintf(FILE *file, yio_printdata_t *data, va_list *va) {
	return yvbprintf(_yIO_yvfprintf_cb, file, data, va);
}

int yvsprintf(Ychar *dest, size_t size, yio_printdata_t *data, va_list *va) {
	struct _yIO_yvsprintf_ctx_s ctx = {
			.dest = dest,
			.size = size,
	};
	const int ret = yvbprintf(_yIO_yvsprintf_cb, &ctx, data, va);
	if (size > 0) {
		ctx.dest[0] = Yc('\0');
	}
	return ret;
}

int yvaprintf(Ychar **strp, yio_printdata_t *data, va_list *va) {
	*strp = NULL;
	return yvreaprintf(strp, data, va);
}

int yvreaprintf(Ychar **strp, yio_printdata_t *data, va_list *va) {
	struct _yIO_yreaprintf_ctx_s ctx = {
			.str = *strp,
			.size = (*strp != NULL ? strlen(*strp) : 0),
	};
	const int ret =  yvbprintf(_yIO_yreaprintf_cb, &ctx, data, va);
	*strp = ctx.str;
	if (ctx.str != NULL) {
		ctx.str[ctx.size] = '\0';
	}
	return ret;
}

Ychar *yvformatf(yio_printdata_t *data, va_list *va) {
	return yvreformatf(NULL, data, va);
}

Ychar *yvreformatf(Ychar *str, yio_printdata_t *data, va_list *va) {
	const int ret = yvreaprintf(&str, data, va);
	if (ret < 0) {
		return NULL;
	}
	return str;
}

/* Private Scan Callback --------------------------------------------------- */

static
int _yIO_yfscanf_cb(void *arg, Yint *data) {
	FILE *f = arg;
	*data = Yfgetc(f);
	if (*data == YEOF) {
		if (ferror(f)) {
			return YIO_ERROR_EIO;
		}
	}
	return 0;
}

static
int _yIO_ysscanf_cb(void *arg, int *data) {
	char * restrict * restrict src = arg;
	*data = (*src)[0];
	if (*data == Yc('\0')) {
		*data = YEOF;
	} else {
		++(*src);
	}
	return 0;
}

/* Private Scan Symbols ------------------------------------------------------ */

struct yio_scanret_s _yIO_ybscanf(_yIO_scancb_t *in, void *arg, yio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yio_scanret_s ret = yvbscanf(in, arg, data, &va);
	va_end(va);
	return ret;
}

struct yio_scanret_s _yIO_yscanf(yio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yio_scanret_s ret = yvfscanf(stdin, data, &va);
	va_end(va);
	return ret;
}

struct yio_scanret_s _yIO_yfscanf(FILE *file, yio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yio_scanret_s ret = yvfscanf(file, data, &va);
	va_end(va);
	return ret;
}

struct yio_scanret_s _yIO_ysscanf(char *src, yio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yio_scanret_s ret = yvsscanf(src, data, &va);
	va_end(va);
	return ret;
}

/* Exported Print Symbols Except yvbscanf -------------------------------------- */

struct yio_scanret_s yvscanf(yio_scandata_t *data, va_list *va) {
#ifdef YIO_USE_INPUT_FD
	return _yIO_ydscanf(0, data, va);
#else
	return _yIO_yfscanf(stdin, data, va);
#endif
}

struct yio_scanret_s yvfscanf(FILE *file, yio_scandata_t *data, va_list *va) {
	return yvbscanf(_yIO_yfscanf_cb, file, data, va);
}

struct yio_scanret_s yvsscanf(char *src, yio_scandata_t *data, va_list *va) {
	return yvbscanf(_yIO_ysscanf_cb, &src, data, va);
}
