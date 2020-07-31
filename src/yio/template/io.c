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
#include <strings.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* _yIO_y*printf ------------------------------------------------------------------------ */

int _yΩIO_ybprintf(_yΩIO_printcb_t *out, void *arg, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvbprintf(out, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yprintf(yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvprintf(data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yfprintf(FILE *file, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_ysprintf(Ychar *dest, size_t size, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvsprintf(dest, size, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yaprintf(Ychar **strp, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

int _yΩIO_yreaprintf(Ychar **strp, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvreaprintf(strp, data, fmt, &va);
	va_end(va);
	return ret;
}

Ychar *_yΩIO_yformatf(yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Ychar * const ret = yπvformatf(data, fmt, &va);
	va_end(va);
	return ret;
}

Ychar *_yΩIO_yreformatf(Ychar *str, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Ychar * const ret = yπvreformatf(str, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Callbacks and contexts ----------------------------------------------------- */

static inline _yIO_access(__read_only__, 2, 3) _yIO_wur _yIO_nn()
size_t _yΩIO_fwrite(FILE *file, const Ychar* str, size_t size) {
m4_template_chooser(`m4_dnl);
	return fwrite(str, 1, size, file);
~,`m4_dnl;
	for (size_t n = 0; n < size; n++) {
		if (fputwc(str[n], file) == YEOF) {
			return n;
		}
	}
	return size;
~,`m4_dnl;
	const bool isnormal = fwide(file, 0) <= 0;
	size_t r = 0;
	if (isnormal) {
		char *mb; size_t mb_len;
		int ret = _yIO_conv_c32s_to_mbs(str, size, &mb, &mb_len);
		if (ret) return ret;
		size_t r = fwrite(mb, 1, mb_len, file);
		free(mb);
		if (r != mb_len) return -1;
	} else {
		wchar_t *wc; size_t wc_len;
		int ret = _yIO_conv_c32s_to_wcs(str, size, &wc, &wc_len);
		if (ret) return ret;
		for (size_t i = wc_len; i--; ) {
			if (fputwc(wc[i], file) == YEOF) {
				free(wc);
				return -1;
			}
		}
		free(wc);
		if (r != wc_len) return -1;
	}
	return size;
~)m4_dnl;
}

static _yIO_access(__read_only__, 2, 3) _yIO_wur _yIO_nn()
int _yΩIO_yvfprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = _yΩIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}

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

int yπvprintf(yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
#ifdef YIO_USE_OUTPUT_FD
	return yπvdprintf(1, data, fmt, va);
#else
	return yπvfprintf(stdout, data, fmt, va);
#endif
}

int yπvfprintf(FILE *file, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	return yπvbprintf(_yΩIO_yvfprintf_cb, file, data, fmt, va);
}

int yπvsprintf(Ychar *dest, size_t size, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
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

int yπvaprintf(Ychar **strp, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	*strp = NULL;
	return yπvreaprintf(strp, data, fmt, va);
}

int yπvreaprintf(Ychar **strp, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
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

Ychar *yπvformatf(yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	return yπvreformatf(NULL, data, fmt, va);
}

Ychar *yπvreformatf(Ychar *str, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	const int ret = yπvreaprintf(&str, data, fmt, va);
	if (ret < 0) {
		return NULL;
	}
	return str;
}

/* Private Scan Callback --------------------------------------------------- */

static
int _yΩIO_yfscanf_cb(void *arg, Yint *data) {
	FILE *f = arg;
	*data = fgetc(f);
	if (*data == YEOF) {
		if (ferror(f)) {
			return YIO_ERROR_EIO;
		}
	}
	return 0;
}

static
int _yΩIO_ysscanf_cb(void *arg, Yint *data) {
	Ychar * restrict * restrict src = arg;
	*data = (*src)[0];
	if (*data == Yc('\0')) {
		*data = YEOF;
	} else {
		++(*src);
	}
	return 0;
}

/* Private Scan Symbols ------------------------------------------------------ */

struct yπio_scanret_s _yΩIO_ybscanf(_yΩIO_scancb_t *in, void *arg, yπio_scandata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const struct yπio_scanret_s ret = yπvbscanf(in, arg, data, fmt, &va);
	va_end(va);
	return ret;
}

struct yπio_scanret_s _yΩIO_yscanf(yπio_scandata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const struct yπio_scanret_s ret = yπvfscanf(stdin, data, fmt, &va);
			// yvscanf(data, fmt, &va);
	va_end(va);
	return ret;
}

struct yπio_scanret_s _yΩIO_yfscanf(FILE *file, yπio_scandata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const struct yπio_scanret_s ret = yπvfscanf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

struct yπio_scanret_s _yΩIO_ysscanf(const Ychar *src, yπio_scandata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const struct yπio_scanret_s ret = yπvsscanf(src, data, fmt, &va);
	va_end(va);
	return ret;
}

/* Exported Print Symbols Except yvbscanf -------------------------------------- */

struct yπio_scanret_s yπvscanf(yπio_scandata_t *data, const Ychar *fmt, va_list *va) {
#ifdef YIO_USE_INPUT_FD
	return _yΩIO_ydscanf(0, data, fmt, va);
#else
	return _yΩIO_yfscanf(stdin, data, fmt, va);
#endif
}

struct yπio_scanret_s yπvfscanf(FILE *file, yπio_scandata_t *data, const Ychar *fmt, va_list *va) {
	return yπvbscanf(_yΩIO_yfscanf_cb, file, data, fmt, va);
}

struct yπio_scanret_s yπvsscanf(const Ychar *src, yπio_scandata_t *data, const Ychar *fmt, va_list *va) {
	return yπvbscanf(_yΩIO_ysscanf_cb, &src, data, fmt, va);
}


