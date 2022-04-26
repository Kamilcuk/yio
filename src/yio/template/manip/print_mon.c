/**
 * @file
 * @date 2022-04-25
 * @author kamil cukrowski
 * @copyright
 * spdx-license-identifier: gpl-3.0-only
 * @brief
 */
#include "private.h"
#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if 0 && _yIO_HAS_MONETARY_H
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <monetary.h>

static inline
int _yIO_alloc_strfmon(char **strp, size_t bufsize, const char *fmt, double v) {
	assert(strp);
	assert(*strp);
	assert(bufsize);
	assert(fmt);

	while (1) {
		errno = 0;
		const ssize_t count = strftime(buf, bufsize, fmt, v);
		if (count != -1) {
			break;
		} else if (errno != 0 && errno != E2BIG) {
			if (buf != *strp) {
				free(buf);
			}
			return YIO_ERROR_STRFTIME;
		}

		// Golden ratio resize
		assert((ssize_t)SSIZE_MAX / 25 > bufsize);
		bufsize *= 25;
		bufsize /= 16;

		if (buf == *strp) {
			buf = NULL;
		}
		void *p = realloc(buf, bufsize);
		if (p == NULL) {
			if (buf != *strp) {
				free(buf);
			}
			return YIO_ERROR_ENOMEM;
		}
		buf = p;
	}
	}
	//
	assert(count <= INT_MAX);
	*strp = buf;
	return count;
}


int print_mon(yπio_printctx_t *t) {
	const double vv = yπio_printctx_va_arg(t, double);
	const YCHAR *const fmtbegin = t->fmt;
	while (t->fmt != Yc('}') && t->fmt != Yc('\0')) t->fmt++;
	if (t->fmt != Yc('\0')) {
		return YIO_ERROR_MISSING_RIGHT_BRACE;
	}
	const YCHAR *const fmt end = t->fmt;
	const size_t realfmtlen = fmtend - fntbegin;
	int err = yπio_printctx_init(t);
	if (err) return err;
	//
	char *format = malloc(sizeof(*format) * (realfmtlen + 1));
	if (format == NULL) {
		return YIO_ERROR_ENOMEM;
	}
	memcpy(format, begin, realfmtlen);
	format[realfmtlen - 1] = Yc('\0');
	//char *formatbuf = NULL;
	//int ret = _yIO_strconv_πstr_to_str(fmtbegin, realfmtlen, (const char **)&formatbuf, NULL);
	//if (ret) return ret;
	char _buf[80];
	char *buf = _buf;
	ret = _yIO_alloc_strfmon(&buf, sizeof(_buf), format, vv);
	free(format);
	if (ret) return ret;
	return _yIO_printfmt_put(buf, ret);
}

#endif
