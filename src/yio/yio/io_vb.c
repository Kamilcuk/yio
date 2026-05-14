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
#include <string.h>

/* yio_vbprintf helpers ------------------------------------------------------ */

static inline
int YIO_yio_vbprintf_iterate_until_format_callback(yio_printctx_t *t, const char *begin, const char *end) {
	//fprintf(stderr, "PO:out=`%.*s`\n", (int)(end - begin), begin);
	return yio_printctx_raw_write(t, begin, (size_t)(end - begin));
}

static inline YIO_nn(1, 2, 3)
int YIO_yio_vbgeneric_iterate_until_format(yio_printctx_t *__single t, const char *__null_terminated fmt, const char *__bidi_indexable *__single endptr) {
	assert(fmt != NULL);
	const char *pos = fmt;
	while (fmt[0] != '\0') {
		fmt += strcspn(fmt, "{}");
		if (fmt[0] == '\0') {
			break;
		}
		if (fmt[0] == fmt[1]) {
			// double { { or } }
			if (fmt != pos) {
				// If we are at start, we can flush already known characters,
				// and continue one after.
				const int err = YIO_yio_vbprintf_iterate_until_format_callback(t, pos, fmt + 1);
				if (err) return err;
				pos = fmt + 2;
				fmt = pos;
			} else {
				// We can optimize a bit - continue from the next character.
				pos += 1;
				fmt = pos + 1;
			}
			continue;
		}
		if (fmt[0] == '}') {
			return YIO_ERROR(YIO_ERROR_SINGLE_RIGHT_BRACE, "single '}' found ousidef of format specifier");
		}
		// {} or {:stuff} found
		break;
	}
	if (fmt != pos) {
		// Flush skipped characters up until now.
		const int err = YIO_yio_vbprintf_iterate_until_format_callback(t, pos, fmt);
		if (err) return err;
	}
	//
	*endptr = fmt;
	return 0;
}

/* yio_vbprintf ----------------------------------------------------------- */

static inline
int YIO_yio_vbprintf_in(yio_printctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->pf = YIO_printfmt_zero;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}
	int err = 0;
#if YIO_ENABLE_DYNAMIC_PFMT
	YIO_skipper skipper = {0};
#endif
	while (1) {
		err = YIO_yio_vbgeneric_iterate_until_format(t, t->fmt, &t->fmt);
		if (err) return err;
		if (t->fmt[0] == '\0') break;
		assert(t->fmt[0] == '{');
		t->fmt++;
		//
		t->pf = YIO_printfmt_zero;
#if YIO_ENABLE_DYNAMIC_PFMT
		if (YIO_isdigit(t->fmt[0])) {
			const unsigned count = YIO_printctx_strtou_noerr(&t->fmt);
			err = YIO_skipper_do(&skipper, t, count);
			if (err) goto EXIT;
		}
#endif
		if (t->fmt[0] == ':') {
			t->fmt++;
		} else if (t->fmt[0] != '}') {
			err = YIO_ERROR_PYFMT_INVALID;
			goto EXIT;
		}
		//
		if (t->ifunc == NULL || *t->ifunc == NULL) {
			err = YIO_ERROR_TOO_MANY_FMT;
			goto EXIT;
		}
		err = (*t->ifunc++)(t);
		if (err) goto EXIT;
		YIO_skipper_end(&skipper, t);
	}
	return 0;
EXIT:
	YIO_skipper_end(&skipper, t);
	return err;
}

int yio_vbprintf(YIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	yio_printctx_t ctx = {0};
	ctx.va = va;
	ctx.fmt = fmt;
	ctx.ifunc = data;
	ctx.out = out;
	ctx.outarg = arg;
#if YIO_ENABLE_DYNAMIC_PFMT
	va_list startva;
	va_copy(startva, *va);
	ctx.startva = &startva;
	ctx.startifunc = data;
#endif
	yio_printctx_t * const t = &ctx;
	const int err = YIO_yio_vbprintf_in(t);
#if YIO_ENABLE_DYNAMIC_PFMT
	va_end(startva);
#endif
	if (err) return -abs(err);
	assert(t->writtencnt <= (unsigned)INT_MAX);
	return (int)t->writtencnt;
}

