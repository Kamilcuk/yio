/**
 * @file
 * @date 2022-04-25
 * @author kamil cukrowski
 * @copyright
 * spdx-license-identifier: gpl-3.0-only
 * @brief
 */
#include "private.h"
#ifndef YYIO_HAS_MONETARY_H
#error
#endif
#if YYIO_HAS_MONETARY_H
#include "../../private/yio_allochelp.h"
#include "../../private/yio_string.h"
#include "print_mon.h"
#include <monetary.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int YYIO_print_mon(yio_printctx_t *t) {
	const double vv = yio_printctx_va_arg(t, double);
	const char *const fmtbegin = t->fmt;
	while (t->fmt[0] != '}' && t->fmt[0] != '\0') t->fmt++;
	if (t->fmt[0] != '}') {
		return YYIO_ERROR(YIO_ERROR_MON_MISSING_RIGHT_BRACE, "missing '}' when parsing monetary format specifier");
	}
	const char *const fmtend = t->fmt;
	int err = yio_printctx_init(t);
	if (err) return err;
	// Prepare format string for strfmon.
	const size_t realfmtlen = fmtend - fmtbegin;
	YYIO_string fmtbuf;
	YYIO_string_init(&fmtbuf);
	err = YYIO_string_reserve(&fmtbuf, realfmtlen + 1);
	if (err) return err;
	char *format = YYIO_string_data(&fmtbuf);
	memcpy(format, fmtbegin, realfmtlen);
	format[realfmtlen] = '\0';
	// Call astrfmon.
	const struct YYIO_astrfmon_arg arg = {
		.v.d = vv,
		.isldbl = false,
	};
	YYIO_string res;
	YYIO_string_init(&res);
	err = YYIO_astrfmon(&res, format, arg);
	YYIO_string_fini(&fmtbuf);
	if (err == 0) {
		err = yio_printctx_put(t, YYIO_string_data(&res), YYIO_string_len(&res));
	}
	YYIO_string_fini(&res);
	return err;
}

#endif
