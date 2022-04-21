/**
 * @file
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <stdlib.h>
#include <assert.h>

/* yvbprintf helpers ------------------------------------------------------ */

static inline
int yπvbprintf_iterate_until_format_callback(yπio_printctx_t *t, const Ychar *begin, const Ychar *end) {
	//fprintf(stderr, "PO:out=`%.*s`\n", (int)(end - begin), begin);
	return yπio_printctx_raw_write(t, begin, end - begin);
}

static inline
int _yΩIO_yvbgeneric_iterate_until_format(yπio_printctx_t *t, const Ychar *fmt, const Ychar **endptr) {
	const Ychar *pos = fmt;
	while (fmt[0] != Yc('\0')) {
		//fprintf(stderr, "P1:pos=`%s` fmt=`%s`\n", pos, fmt);
		if (fmt[0] == Yc('{') || fmt[0] == Yc('}')) {
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
				if (fmt[0] == Yc('}')) {
					return YIO_ERROR_SINGLE_RIGHT_BRACE;
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
		if (t->fmt[0] == Yc('\0')) break;
		assert(t->fmt[0] == Yc('{'));
		t->fmt++;

		t->pf = _yΩIO_printfmt_default;
		// Handle conversion specifier.
		if (t->fmt[0] == Yc('!')) {
			t->fmt++;
			if (t->fmt[0] != Yc('s')) {
				return YIO_ERROR_UNKNOWN_CONVERSION;
			}
			t->pf.conversion = t->fmt[0];
			t->fmt++;
		}
		if (t->fmt[0] == Yc(':')) {
			t->fmt++;
		} else if (t->fmt[0] != Yc('}')) {
			return YIO_ERROR_PYFMT_INVALID;
		}

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

int yπvbprintf(_yΩIO_printcb_t *out, void *arg, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	yπio_printctx_t _ctx;
	yπio_printctx_t * const t = &_ctx;
	_yΩIO_printctx_init(t, out, arg, data, fmt, va);
	const int err = yπvbprintf_in(t);
	if (err) {
		return -abs(err);
	}
	assert(t->writtencnt < (size_t)INT_MAX);
	return t->writtencnt;
}

