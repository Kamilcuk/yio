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
#include "../../private/yio_strlib.h"
#include "print_mon.h"
#include <monetary.h>
#include <assert.h>
#include <errno.h>
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
	//
	const size_t realfmtlen = fmtend - fmtbegin;
	char *format;
#if 1 == 1
	format = malloc(sizeof(*format) * (realfmtlen + 1));
	if (format == NULL) return YIO_ERROR_ENOMEM;
	memcpy(format, fmtbegin, realfmtlen);
	format[realfmtlen] = '\0';
#elif 1 == 2 || 1 == 3 || 1 == 4
	err = YYIO_strconv_str_to_str(fmtbegin, realfmtlen, (const char **)&format, NULL);
	if (err) return err;
#else
#error
#endif
	const struct YYIO_astrfmon_arg arg = {
		.v.d = vv,
		.isldbl = false,
	};
	YYIO_RES_AUTO_DECL(res);
	err = YYIO_astrfmon(&res, format, arg);
	free(format);
	if (err == 0) {
		err = yio_printctx_put(t, YYIO_res_begin(&res), YYIO_res_used(&res));
	}
	YYIO_res_end(&res);
	return err;
}

#endif
