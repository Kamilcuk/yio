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
int YYIO_yio_vbprintf_iterate_until_format_callback(yio_printctx_t *t, const char *begin, const char *end) {
	//fprintf(stderr, "PO:out=`%.*s`\n", (int)(end - begin), begin);
	return yio_printctx_raw_write(t, begin, (size_t)(end - begin));
}

static inline YYIO_nn(1, 2, 3)
int YYIO_yio_vbgeneric_iterate_until_format(yio_printctx_t *__single t, const char *__null_terminated fmt, const char *__bidi_indexable *__single endptr) {
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
				const int err = YYIO_yio_vbprintf_iterate_until_format_callback(t, pos, fmt + 1);
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
			return YYIO_ERROR(YIO_ERROR_SINGLE_RIGHT_BRACE, "single '}' found ousidef of format specifier");
		}
		// {} or {:stuff} found
		break;
	}
	if (fmt != pos) {
		// Flush skipped characters up until now.
		const int err = YYIO_yio_vbprintf_iterate_until_format_callback(t, pos, fmt);
		if (err) return err;
	}
	//
	*endptr = fmt;
	return 0;
}

/* yio_vbprintf ----------------------------------------------------------- */

static inline
int YYIO_yio_vbprintf_in(yio_printctx_t *t) {
	if (t->fmt == NULL) {
		if (t->ifunc == NULL) {
			return 0;
		}
		for (; *t->ifunc != NULL; ++t->ifunc) {
			t->pf = YYIO_printfmt_zero;
			const int ifuncret = (*t->ifunc)(t);
			if (ifuncret) {
				return ifuncret;
			}
		}
		return 0;
	}
	while (1) {
		int err = YYIO_yio_vbgeneric_iterate_until_format(t, t->fmt, &t->fmt);
		if (err) return err;
		if (t->fmt[0] == '\0') break;
		assert(t->fmt[0] == '{');
		t->fmt++;
		//
		t->pf = YYIO_printfmt_zero;
		if (YYIO_isdigit(t->fmt[0])) {
			YYIO_skip_arm(t, YYIO_printctx_strtou_noerr(&t->fmt));
		}
		#if 0
		// Handle conversion specifier - currently disabled.
		if (t->fmt[0] == '!') {
			t->fmt++;
			if (t->fmt[0] != 'a') {
				return YIO_ERROR_UNKNOWN_CONVERSION;
			}
			t->pf.c_onversion = t->fmt[0];
			t->fmt++;
		}
		#endif
		if (t->fmt[0] == ':') {
			t->fmt++;
		} else if (t->fmt[0] != '}') {
			return YIO_ERROR_PYFMT_INVALID;
		}
		const int skipret = YYIO_skip_do(t);
		if (skipret) return skipret;
		if (t->ifunc == NULL || *t->ifunc == NULL) {
			return YIO_ERROR_TOO_MANY_FMT;
		}
		const int ifuncret = (*t->ifunc++)(t);
		if (ifuncret) return ifuncret;
	}
	return 0;
}

int yio_vbprintf(YYIO_printcb_t *out, void *arg, const yio_printdata_t *data, const char *fmt, va_list *va) {
	assert(out != NULL);
	assert(data != NULL);
	assert(va != NULL);
	yio_printctx_t ctx = {0};
	ctx.va = va;
	ctx.fmt = fmt;
	ctx.ifunc = data;
	ctx.startifunc = data;
	ctx.out = out;
	ctx.outarg = arg;
#if YIO_ENABLE_DYNAMIC_PFMT
	va_list startva;
	va_copy(startva, *va);
	ctx.startva = &startva;
#endif
	yio_printctx_t * const t = &ctx;
	const int err = YYIO_yio_vbprintf_in(t);
#if YIO_ENABLE_DYNAMIC_PFMT
	va_end(startva);
#endif
	if (err) return -abs(err);
	assert(t->writtencnt <= (unsigned)INT_MAX);
	return (int)t->writtencnt;
}

