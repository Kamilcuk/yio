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

/* yio_vbprintf helpers ------------------------------------------------------ */

static inline
int YYIO_yio_vbprintf_iterate_until_format_callback(yio_printctx_t *t, const char *begin, const char *end) {
	//fprintf(stderr, "PO:out=`%.*s`\n", (int)(end - begin), begin);
	return yio_printctx_raw_write(t, begin, (size_t)(end - begin));
}

static inline
int YYIO_yio_vbgeneric_iterate_until_format(yio_printctx_t *t, const char fmt[restrict], const char **restrict endptr) {
	const char *pos = fmt;
	while (fmt[0] != '\0') {
		//fprintf(stderr, "P1:pos=`%s` fmt=`%s`\n", pos, fmt);
		if (fmt[0] == '{' || fmt[0] == '}') {
			if (fmt[0] == fmt[1]) {
				// double { { or } }
				if (fmt != pos) {
					//fprintf(stderr, "P2\n");
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
		fmt++;
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
			t->pf = YYIO_printfmt_default;
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
		t->pf = YYIO_printfmt_default;
		if (isdigit((unsigned char)(t->fmt[0]))) {
			YYIO_skip_arm(t, (unsigned int)YYIO_printctx_strtoi_noerr(&t->fmt));
		}
		if (t->fmt[0] == '!') {
			// Handle conversion specifier.
			t->fmt++;
			if (t->fmt[0] != 'a') {
				return YIO_ERROR_UNKNOWN_CONVERSION;
			}
			t->pf.c_onversion = t->fmt[0];
			t->fmt++;
		}
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
	va_list startva;
	va_copy(startva, *va);
	yio_printctx_t _ctx = {
		.va = va,
		.startva = &startva,
		.fmt = fmt,
		.ifunc = (yio_printdata_t *)data,
		.startifunc = (yio_printdata_t *)data,
		.out = out,
		.outarg = arg,
	};
	yio_printctx_t * const t = &_ctx;
	const int err = YYIO_yio_vbprintf_in(t);
	va_end(startva);
	if (err) {
		return -abs(err);
	}
	assert(t->writtencnt <= (unsigned)INT_MAX);
	return (int)t->writtencnt;
}

