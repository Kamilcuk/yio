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
#if _yIO_HAS_MONETARY_H
#include "../../private/yio_allochelp.h"
#include "../../private/yio_strlib.h"
#include "print_mon.h"
#include <monetary.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int _yΩIO_print_mon(yπio_printctx_t *t) {
	const double vv = yπio_printctx_va_arg(t, double);
	const TCHAR *const fmtbegin = t->fmt;
	while (t->fmt[0] != TC('}') && t->fmt[0] != TC('\0')) t->fmt++;
	if (t->fmt[0] != TC('}')) {
		return _yIO_ERROR(YIO_ERROR_MON_MISSING_RIGHT_BRACE, "missing '}' when parsing monetary format specifier");
	}
	const TCHAR *const fmtend = t->fmt;
	int err = yπio_printctx_init(t);
	if (err) return err;
	//
	const size_t realfmtlen = fmtend - fmtbegin;
	char *format;
#if TMODE == 1
	format = malloc(sizeof(*format) * (realfmtlen + 1));
	if (format == NULL) return YIO_ERROR_ENOMEM;
	memcpy(format, fmtbegin, realfmtlen);
	format[realfmtlen] = TC('\0');
#elif TMODE == 2 || TMODE == 3 || TMODE == 4
	err = _yIO_strconv_πstr_to_str(fmtbegin, realfmtlen, (const char **)&format, NULL);
	if (err) return err;
#else
#error
#endif
	const struct _yIO_astrfmon_arg arg = {
		.v.d = vv,
		.isldbl = false,
	};
	_yIO_RES_AUTO_DECL(res);
	err = _yIO_astrfmon(&res, format, arg);
	free(format);
	if (err == 0) {
		err = yπio_printctx_put(t, _yIO_res_begin(&res), _yIO_res_used(&res));
	}
	_yIO_res_end(&res);
	return err;
}

#endif
