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

#if YYIO_HAS_timespec || YYIO_HAS_timeval
static inline
int YYIO_print_timespec_val(yio_printctx_t *t, long long sec, long long nsec, int precision_default) {
	struct yio_printfmt_s *const pf = &t->pf;
	const char type = pf->type;
	const int precision = (int)yio_precision_get_default(pf->precision, (size_t)precision_default);

	YYIO_string res;
	YYIO_string_init(&res);

	const bool is_neg = sec < 0 || nsec < 0;
	const unsigned long long abs_sec = (unsigned long long)(sec < 0 ? -(unsigned long long)sec : (unsigned long long)sec);
	const unsigned long long abs_nsec = (unsigned long long)(nsec < 0 ? -(unsigned long long)nsec : (unsigned long long)nsec);

	int err = 0;
	unsigned long long fraction = abs_nsec;
	if (precision < precision_default) {
		unsigned long long divisor = 1;
		for (int i = 0; i < precision_default - precision; ++i) divisor *= 10;
		fraction /= divisor;
	} else if (precision > precision_default) {
		for (int i = 0; i < precision - precision_default; ++i) fraction *= 10;
	}

	if (is_neg) {
		err = YYIO_string_putc(&res, '-');
		if (err) goto end;
	}

	if (type == 'f' || type == 'g') {
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){0}, abs_sec);
		if (err) goto end;
		if (precision > 0 || pf->hash) {
			err = YYIO_string_putc(&res, '.');
			if (err) goto end;
			if (precision > 0) {
				err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = (uint16_t)precision + 1, .align = '=', .fill = '0'}, fraction);
				if (err) goto end;
			}
		}
	} else if (type == '\0') {
		const unsigned long long h = abs_sec / 3600;
		const unsigned long long m = (abs_sec % 3600) / 60;
		const unsigned long long s = abs_sec % 60;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){0}, h);
		if (err) goto end;
		err = YYIO_string_putc(&res, ':');
		if (err) goto end;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = 3, .align = '=', .fill = '0'}, m);
		if (err) goto end;
		err = YYIO_string_putc(&res, ':');
		if (err) goto end;
		err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = 3, .align = '=', .fill = '0'}, s);
		if (err) goto end;
		if (precision > 0 || pf->hash) {
			err = YYIO_string_putc(&res, '.');
			if (err) goto end;
			if (precision > 0) {
				err = YYIO_string_print_ull_in(&res, (struct yio_printfmt_s){.width = (uint16_t)precision + 1, .align = '=', .fill = '0'}, fraction);
				if (err) goto end;
			}
		}
	} else {
		err = YIO_ERROR_INVALID_TYPE;
		goto end;
	}

	if (type == 'g' || (type == '\0' && pf->hash)) {
		const size_t len = YYIO_string_len(&res);
		if (len > 0) {
			char * const data = YYIO_string_data(&res);
			char *p = data + len - 1;
			while (p > data && *p == '0') {
				--p;
			}
			if (*p == '.') {
				// remove dot
			} else {
				++p;
			}
			YYIO_string_set_used(&res, (size_t)(p - data));
		}
	}
	err = yio_printctx_put(t, YYIO_string_data(&res), YYIO_string_len(&res));

end:
	YYIO_string_free(&res);
	return err;
}
#endif

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
	return YYIO_print_timespec_val(t, (long long)ts.tv_sec, (long long)ts.tv_nsec, 9);
}
#endif // YYIO_HAS_timespec

#if YYIO_HAS_timeval
int YYIO_print_timeval(yio_printctx_t *t) {
	const struct timeval ts = yio_printctx_va_arg(t, struct timeval);
	int err = yio_printctx_init(t);
	if (err) return err;
	return YYIO_print_timespec_val(t, (long long)ts.tv_sec, (long long)ts.tv_usec, 6);
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
