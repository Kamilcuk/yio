/**
 * @file
 * @date: 2020-lip-02
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#define _POSIX_C_SOURCE  1
#include "private.h"
#include "yio/private/yio_allochelp.h"
#include <assert.h>
#include <time.h>
#if YYIO_HAS_UNISTD_H
#include <sys/time.h>
#endif

// https://fmt.dev/latest/syntax.html#chrono-format-specifications
// chrono_format_spec ::=  [[fill]align][width]["." precision][chrono_specs]
// chrono_specs       ::=  [chrono_specs] conversion_spec | chrono_specs literal_char
// conversion_spec    ::=  "%" [modifier] chrono_type
// literal_char       ::=  <a character other than '{', '}' or '%'>
static inline
int YYIO_print_time_parse_format(yio_printctx_t *t, const char **beginptr, const char **endptr) {
	const char *fmt = t->fmt;
	const char *chrono_specs = fmt;
	struct yio_printfmt_s *const pf = &t->pf;
	//
	if (fmt != NULL) {
		if (fmt[0] != '\0') {
			// guess [[fill]align]
			if (YYIO_ANYEQ(fmt[1], '<', '>', '=', '^')) {
				pf->fill = fmt++[0];
				pf->align = fmt++[0];
			} else if (YYIO_ANYEQ(fmt[0], '<', '>', '=', '^')) {
				pf->align = fmt++[0];
			}
		}
		if (fmt[0] != '\0') {
			// guess width
			const int err = YYIO_printctx_stdintparam(t, fmt, &fmt, &pf->width);
			if (err) return err;
		}
		if (fmt[0] == '.') {
			// guess precision
			fmt++;
			if (fmt[0] == '\0') {
				return YYIO_ERROR(YIO_ERROR_TIME_MISSING_PRECISION, "Missing precision when parsing time format specifier");
			}
			const int err = YYIO_printctx_stdintparam(t, fmt, &fmt, &pf->precision);
			if (err) return err;
		}
		// chrono_specs preserve as-is
		chrono_specs = fmt;
		while (fmt[0] != '\0' && fmt[0] != '}') {
			fmt++;
		}
		if (fmt[0] != '}') {
			return YYIO_ERROR(YIO_ERROR_TIME_MISSING_RIGHT_BRACE, "Missing '}' when parsing time format specifier");
		}
	}
	*beginptr = chrono_specs;
	*endptr = fmt;
	return 0;
}

static inline
int YYIO_print_time_strftime(yio_printctx_t *t, const struct tm *tm) {
	//
	const char *fmtbegin = NULL;
	const char *fmtend = NULL;
	int err = YYIO_print_time_parse_format(t, &fmtbegin, &fmtend);
	if (err) return err;
	// Advance global fmt.
	if (t->fmt) {
		assert(fmtend[0] == '}');
		t->fmt = fmtend;
	}
	// Initialize printctx - after erading format string.
	err = yio_printctx_init(t);
	if (err) return err;
	//
	const ptrdiff_t realfmtlen = fmtend - fmtbegin;

	// Extract the format string.
	// Add additional space.
	YYIO_string fmtbut;
	YYIO_string_init(&fmtbut);
	const char *const emptyformat = "%c ";
	const char *format = NULL;
	if (realfmtlen == 0) {
		// Zero fmt length results in '%c'.
		format = emptyformat;
	} else {
		// Add additional space.
		const ptrdiff_t fmtlen = realfmtlen + 2;
		err = YYIO_string_reserve(&fmtbut, fmtlen);
		if (err) return err;
		char *data = YYIO_string_data(&fmtbut);
		memcpy(data, fmtbegin, realfmtlen);
		data[fmtlen - 2] = ' ';
		data[fmtlen - 1] = '\0';
		format = data;
	}
	assert(format != NULL);
	assert(strlen(format) >= 1);
	assert(format[strlen(format) - 1] == ' ');
	//
	YYIO_string res;
	YYIO_string_init(&res);
	err = YYIO_astrftime_nonzero(&res, format, tm);
	YYIO_string_free(&fmtbut);
	if (err == 0) {
		assert(YYIO_string_len(&res) > 1);
		const size_t reslen = YYIO_string_len(&res) - 1;
		const struct yio_printfmt_s *const pf = &t->pf;
		const size_t toprint = pf->precision == 0 ? reslen :
			reslen < yio_precision_get_default(pf->precision, 0) ? reslen :
			yio_precision_get_default(pf->precision, 0);
		err = yio_printctx_put(t, YYIO_string_data(&res), toprint);
	}
	YYIO_string_free(&res);
	return err;
}

#ifndef __SDCC
int YYIO_print_tm(yio_printctx_t *t) {
	const struct tm tm = yio_printctx_va_arg(t, struct tm);
	return YYIO_print_time_strftime(t, &tm);
}
#endif

int YYIO_print_tm_pointer(yio_printctx_t *t) {
	const struct tm *tm = yio_printctx_va_arg(t, struct tm*);
	if (tm == NULL) return YIO_ERROR_INVALID_TYPE;
	return YYIO_print_time_strftime(t, tm);
}

#if YYIO_HAS_timespec
int YYIO_print_timespec(yio_printctx_t *t) {
	const struct timespec ts = yio_printctx_va_arg(t, struct timespec);
	int err = yio_printctx_init(t);
	if (err) return err;
	return yio_printctx_printf(t, "{}.{:09}", ts.tv_sec, ts.tv_nsec);
}
#endif // YYIO_HAS_timespec

#if YYIO_HAS_timeval
int YYIO_print_timeval(yio_printctx_t *t) {
	const struct timeval ts = yio_printctx_va_arg(t, struct timeval);
	int err = yio_printctx_init(t);
	if (err) return err;
	return yio_printctx_printf(t, "{}.{:06}", ts.tv_sec, ts.tv_usec);
}
#endif // YYIO_HAS_timeval

int YYIO_print_localtime(yio_printctx_t *t) {
	time_t arg = yio_printctx_va_arg(t, time_t);
	struct tm *tm = localtime(&arg);
	if (tm == NULL) return YYIO_ERROR(YIO_ERROR_LOCALTIME, "localtime returned NULL");
	return YYIO_print_time_strftime(t, tm);
}

int YYIO_print_gmtime(yio_printctx_t *t) {
	time_t arg = yio_printctx_va_arg(t, time_t);
	struct tm *tm = gmtime(&arg);
	if (tm == NULL) return YYIO_ERROR(YIO_ERROR_GMTIME, "gmtime returned NULL");
	return YYIO_print_time_strftime(t, tm);
}
