/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "ctx.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

/* yvbprintf helpers ------------------------------------------------------ */

static inline
int yπvbprintf_iterate_until_format_callback(yπio_printctx_t *t, const TCHAR *begin, const TCHAR *end) {
	//fprintf(stderr, "PO:out=`%.*s`\n", (int)(end - begin), begin);
	return yπio_printctx_raw_write(t, begin, end - begin);
}

static inline
int _yΩIO_yvbgeneric_iterate_until_format(yπio_printctx_t *t, const TCHAR *fmt, const TCHAR **endptr) {
	const TCHAR *pos = fmt;
	while (fmt[0] != TC('\0')) {
		//fprintf(stderr, "P1:pos=`%s` fmt=`%s`\n", pos, fmt);
		if (fmt[0] == TC('{') || fmt[0] == TC('}')) {
			if (fmt[0] == fmt[1]) {
				// double { { or } }
				if (fmt != pos) {
					//fprintf(stderr, "P2\n");
					// If we are at start, we can flush already known characters,
					// and continue one after.
					const int err = yπvbprintf_iterate_until_format_callback(t, pos, fmt + 1);
					if (err) return err;
					pos = fmt + 2;
					fmt = pos;
					continue;
				} else {
					// We can optimize a bit - continue from the next character.
					pos += 1;
					fmt = pos + 1;
					continue;
				}
			} else {
				if (fmt[0] == TC('}')) {
					return _yIO_ERROR(YIO_ERROR_SINGLE_RIGHT_BRACE, "single '}' found ousidef of format specifier");
				}
				// {} or {:stuff} found
				break;
			}
		}
		fmt++;
	}
	if (fmt != pos) {
		// Flush skipped characters up until now.
		const int err = yπvbprintf_iterate_until_format_callback(t, pos, fmt);
		if (err) return err;
	}
	//
	*endptr = fmt;
	return 0;
}

/* yvbprintf ----------------------------------------------------------- */

static inline
int yπvbprintf_in(yπio_printctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->pf = _yΩIO_printfmt_default;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}
	while (1) {
		int err = _yΩIO_yvbgeneric_iterate_until_format(t, t->fmt, &t->fmt);
		if (err) return err;
		if (t->fmt[0] == TC('\0')) break;
		assert(t->fmt[0] == TC('{'));
		t->fmt++;
		//
		t->pf = _yΩIO_printfmt_default;
		if (TISDIGIT(t->fmt[0])) {
			_yΩIO_skip_arm(t, _yΩIO_printctx_strtoi_noerr(&t->fmt));
		}
		if (t->fmt[0] == TC('!')) {
			// Handle conversion specifier.
			t->fmt++;
			if (t->fmt[0] != TC('a')) {
				return YIO_ERROR_UNKNOWN_CONVERSION;
			}
			t->pf.conversion = t->fmt[0];
			t->fmt++;
		}
		if (t->fmt[0] == TC(':')) {
			t->fmt++;
		} else if (t->fmt[0] != TC('}')) {
			return YIO_ERROR_PYFMT_INVALID;
		}
		const int skipret = _yΩIO_skip_do(t);
		if (skipret) return skipret;
		if (t->ifunc == NULL || *t->ifunc == NULL) {
			return YIO_ERROR_TOO_MANY_FMT;
		}
		const int ifuncret = (*t->ifunc++)(t);
		if (ifuncret) return ifuncret;
	}
	return 0;
}

int yπvbprintf(_yΩIO_printcb_t *out, void *arg, yπio_printdata_t *data, const TCHAR *fmt, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	va_list startva;
	va_copy(startva, *va);
	yπio_printctx_t _ctx = {
		.va = va,
		.startva = &startva,
		.fmt = fmt,
		.ifunc = data,
		.startifunc = data,
		.out = out,
		.outarg = arg,
	};
	yπio_printctx_t * const t = &_ctx;
	const int err = yπvbprintf_in(t);
	va_end(startva);
	if (err) {
		return -abs(err);
	}
	assert(t->writtencnt <= (unsigned)INT_MAX);
	return t->writtencnt;
}

