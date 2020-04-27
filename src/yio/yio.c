/**
 * @file yio.c
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_include(cfg_yio.m4)
#include "yio.h"
#include "yio_private.h"
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

static
int _yIO_yvfprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = fwrite(ptr, 1, size, f);
	if (cnt != size) {
		return YIO_ERROR_EIO;
	}
	return 0;
}

struct _yIO_yvsprintf_ctx_s {
	Ychar *dest;
	Ychar *end;
};

static
int _yIO_yvsprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	struct _yIO_yvsprintf_ctx_s *c = arg;
	if (c->dest == NULL) {
		return 0;
	}
	assert(SIZE_MAX - 1 > size);
	assert(c->end > c->dest);
	const bool not_enough_space =
			(size_t)(c->end - c->dest) < size + 1;
	size = not_enough_space ? (size_t)(c->end - c->dest) - 1 : size;
	(void)memcpy(c->dest, ptr, size);
	c->dest += size;
	assert(c->dest < c->end);
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
	return yvfprintf(stdout, data, va);
}

int yvfprintf(FILE *file, yio_printdata_t *data, va_list *va) {
	return yvbprintf(_yIO_yvfprintf_cb, file, data, va);
}

int yvsprintf(Ychar *dest, size_t size, yio_printdata_t *data, va_list *va) {
	struct _yIO_yvsprintf_ctx_s ctx = {
			.dest = dest,
			.end = dest + size,
	};
	const int ret = yvbprintf(_yIO_yvsprintf_cb, &ctx, data, va);
	if (size > 0) {
		ctx.dest[0] = '\0';
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

int _yIO_yfscanf_cb(void *arg, Yint *data) {
	FILE *f = arg;
	*data = fgetc(f);
	if (*data == YEOF) {
		if (ferror(f)) {
			return YIO_ERROR_EIO;
		}
	}
	return 0;
}

int _yIO_ysscanf_cb(void *arg, int *data) {
	char * restrict * restrict src = arg;
	*data = (*src)[0];
	if (*data == Ychar_constant('\0')) {
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
	return _yIO_yfscanf(stdin, data, va);
}

struct yio_scanret_s yvfscanf(FILE *file, yio_scandata_t *data, va_list *va) {
	return yvbscanf(_yIO_yfscanf_cb, file, data, va);
}

struct yio_scanret_s yvsscanf(char *src, yio_scandata_t *data, va_list *va) {
	return yvbscanf(_yIO_ysscanf_cb, &src, data, va);
}

/* yvbprintf and yvbscanf helpers ------------------------------------------------------ */

static inline _yIO_wur _yIO_nn() _yIO_rnn
const Ychar *_yIO_strpbrk_braces(const Ychar *str) {
	while (*str) {
		if (*str == Ychar_constant('{') || *str == Ychar_constant('}')) {
			break;
		}
		++str;
	}
	return str;
}

static inline
int _yIO_yvbgeneric_iterate_until_format(const Ychar *fmt, const Ychar **endptr,
		int (*callback)(void *arg, const Ychar *begin, const Ychar *end), void *arg) {
	bool double_braces_found = false;
	while (fmt[0]) {
		const Ychar * const after_brace = fmt + double_braces_found;
		const Ychar * const brace = _yIO_strpbrk_braces(after_brace);
		if (after_brace != brace) {
			const int err = callback(arg, fmt, brace);
			if (err) return err;
			fmt = brace;

			if (fmt[0] == Ychar_constant('\0')) {
				break;
			}
		}
		fmt = brace;

		assert(fmt[0] == Ychar_constant('{') || fmt[0] == Ychar_constant('}'));
		double_braces_found = fmt[0] == fmt[1];
		if (double_braces_found) {
			++fmt;
			continue;
		}
		break;
	}
	*endptr = fmt;
	return 0;
}

static inline
int yvbprintf_iterate_until_format_callback(void *arg, const Ychar *begin, const Ychar *end) {
	yio_printctx_t *t = arg;
	return yio_printctx_out(t, begin, end - begin);
}

/* yvbprintf ----------------------------------------------------------- */

static inline
int yvbprintf_in(yio_printctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->pf = _yIO_printfmt_default;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}

	while (1) {
		int err = _yIO_yvbgeneric_iterate_until_format(t->fmt, &t->fmt,
				yvbprintf_iterate_until_format_callback, t);
		if (err) return err;
		if (t->fmt[0] == Ychar_constant('\0')) break;

		err = _yIO_pfmt_parse(&t->c, &t->pf, t->fmt, &t->fmt);
		if (err) return err;

		if (t->ifunc == NULL || *t->ifunc == NULL) {
			return YIO_ERROR_TOO_MANY_FMT;
		}
		const int ifuncret = (*t->ifunc)(t);
		if (ifuncret) {
			return ifuncret;
		}
		t->ifunc++;

	}
	return 0;
}

int yvbprintf(_yIO_printcb_t *out, void *arg, yio_printdata_t *data, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	yio_printctx_t _ctx = _yIO_printctx_init(out, arg, data, va);
	yio_printctx_t * const t = &_ctx;
	const int err = yvbprintf_in(t);
	if (err) {
		return -abs(err);
	}
	if (t->writtencnt > INT_MAX) {
		return INT_MAX;
	}
	return t->writtencnt;
}

/* yvbscan ----------------------------------------------------------------------------- */

static inline
int _yIO_yvbscanf_iterate_until_format_callback(void *arg, const char *begin, const char *end) {
	yio_scanctx_t *t = arg;
	return _yIO_scan_string_literal_in(t, begin, end);
}

static inline
int _yIO_yvbscanf_in(yio_scanctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->sf = _yIO_scanfmt_default;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}

	while (1) {
		int err = _yIO_yvbgeneric_iterate_until_format(t->fmt, &t->fmt,
				_yIO_yvbscanf_iterate_until_format_callback, t);
		if (err) return err;
		if (t->fmt[0] == Ychar_constant('\0')) break;

		err = _yIO_scan_parse_scanfmt(&t->c, &t->sf, t->fmt, &t->fmt);
		if (err) return err;

		if (t->sf.ignore && t->sf.type) {
			// not implemented yet
			return YIO_ERROR_ENOSYS;
		} else {
			if (t->ifunc == NULL || *t->ifunc == NULL) {
				return YIO_ERROR_TOO_MANY_FMT;
			}
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
			t->ifunc++;
		}
	}
	return 0;
}

struct yio_scanret_s yvbscanf(_yIO_scancb_t *in, void *arg,
		yio_scandata_t *data,
		va_list *va) {
	yio_scanctx_t _ctx = _yIO_scanctx_init(in, arg, data, va);
	yio_scanctx_t * const t = &_ctx;
	const int err = _yIO_yvbscanf_in(t);
	const struct yio_scanret_s scanret = {
			.error = err,
			.count = t->scannedcnt,
			.nitem = t->ifunc - data->funcs,
			.lastchar = t->ch,
	};
	return scanret;
}
