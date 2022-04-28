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
#if _yIO_HAS_UNISTD_H
#include <sys/time.h>
#endif

// https://fmt.dev/latest/syntax.html#chrono-format-specifications
// chrono_format_spec ::=  [[fill]align][width]["." precision][chrono_specs]
// chrono_specs       ::=  [chrono_specs] conversion_spec | chrono_specs literal_char
// conversion_spec    ::=  "%" [modifier] chrono_type
// literal_char       ::=  <a character other than '{', '}' or '%'>
static inline
int _yΩIO_print_time_parse_format(yπio_printctx_t *t, const TCHAR **beginptr, const TCHAR **endptr) {
	const TCHAR *fmt = t->fmt;
	const TCHAR *chrono_specs = fmt;
	struct yπio_printfmt_s *const pf = &t->pf;
	//
	if (fmt != NULL) {
		if (fmt[0] != TC('\0')) {
			// guess [[fill]align]
			if (_yΩIO_strnulchrbool(TC("<>=^"), fmt[1])) {
				pf->fill = fmt++[0];
				pf->align = fmt++[0];
			} else if (_yΩIO_strnulchrbool(TC("<>=^"), fmt[0])) {
				pf->align = fmt++[0];
			}
		}
		if (fmt[0] != TC('\0')) {
			// guess width
			const int err = _yΩIO_printctx_stdintparam(t, fmt, &fmt, &pf->width);
			if (err) return err;
		}
		if (fmt[0] == TC('.')) {
			// guess precision
			fmt++;
			if (fmt[0] == TC('\0')) {
				return _yIO_ERROR(YIO_ERROR_TIME_MISSING_PRECISION, "Missing precision when parsing time format specifier");
			}
			const int err = _yΩIO_printctx_stdintparam(t, fmt, &fmt, &pf->precision);
			if (err) return err;
		}
		// chrono_specs preserve as-is
		chrono_specs = fmt;
		while (fmt[0] != TC('\0') && fmt[0] != TC('}')) {
			fmt++;
		}
		if (fmt[0] != TC('}')) {
			return _yIO_ERROR(YIO_ERROR_TIME_MISSING_RIGHT_BRACE, "Missing '}' when parsing time format specifier");
		}
	}
	*beginptr = chrono_specs;
	*endptr = fmt;
	return 0;
}

static inline
int _yΩIO_print_time_strftime(yπio_printctx_t *t, const struct tm *tm) {
	//
	const TCHAR *fmtbegin = NULL;
	const TCHAR *fmtend = NULL;
	int ret = _yΩIO_print_time_parse_format(t, &fmtbegin, &fmtend);
	if (ret) goto FORMAT_EXTRACT_ERROR;
	// Advance global fmt.
	if (t->fmt) {
		assert(fmtend[0] == TC('}'));
		t->fmt = fmtend;
	}
	// Initialize printctx - after erading format string.
	ret = yπio_printctx_init(t);
	if (ret) return ret;
	//
	const ptrdiff_t realfmtlen = fmtend - fmtbegin;

	// Extract the format string.
	// Add additional space.
	const char *const emptyformat = "%c ";
	const char *format = NULL;
	if (realfmtlen == 0) {
		// Zero fmt length results in '%c'.
		format = emptyformat;
	} else {
		// Add additional space.
		const ptrdiff_t fmtlen = realfmtlen + 2;
		char *formatbuf = NULL;
#if TMODE == 1
		formatbuf = malloc(sizeof(*formatbuf) * fmtlen);
		if (formatbuf == NULL) {
			ret = YIO_ERROR_ENOMEM;
			goto FORMAT_MALLOC_ERROR;
		}
		memcpy(formatbuf, fmtbegin, sizeof(*formatbuf) * realfmtlen);
		if (0) goto FORMAT_STRCONV_ERROR;
#else
		if (0) goto FORMAT_MALLOC_ERROR;
		ret = _yIO_strconv_πstr_to_str(fmtbegin, realfmtlen, (const char **)&formatbuf, NULL);
		if (ret) {
			goto FORMAT_STRCONV_ERROR;
		}
		void *pnt = realloc((void *)formatbuf, sizeof(*formatbuf) * fmtlen);
		if (pnt == NULL) {
			ret = YIO_ERROR_ENOMEM;
			goto FORMAT_STRCONV_ERROR;
		}
		formatbuf = pnt;
#endif
		formatbuf[fmtlen - 2] = ' ';
		formatbuf[fmtlen - 1] = '\0';
		format = formatbuf;
	}
	assert(format != NULL);
	assert(strlen(format) >= 1);
	assert(format[strlen(format) - 1] == ' ');

	// 80 is somewhat a psuedo standard here
	char _buf_mem[80];
	char *buf = _buf_mem;
	const int length = _yIO_astrftime_nonzero(&buf, sizeof(_buf_mem), format, tm);
	if (length < 0) {
		ret = length;
		goto STRFTIME_ERROR;
	}

	// We pass length - 1 cause without extra space we allocated in format string.
	// Note that this _can_ result in an empty string here.
	ret = yπio_printctx_put(t, buf, length - 1);

	// exit
	if (buf != _buf_mem) {
		free(buf);
	}
	STRFTIME_ERROR:
	FORMAT_STRCONV_ERROR:
	if (format != emptyformat) {
		free((void *)format);
	}
	FORMAT_MALLOC_ERROR:
	FORMAT_EXTRACT_ERROR:
	return ret;
}

static inline
struct tm _yIO_localtime(const time_t *sec) {
	struct tm tm;
#if _yIO_HAS_localtime_r
	localtime_r(sec, &tm);
#else
	tm = *localtime(sec); // NOLINT(runtime/threadsafe_fn)
#endif
	return tm;
}

static inline
struct tm _yIO_gmtime(const time_t *sec) {
	struct tm tm;
#if _yIO_HAS_localtime_r
	gmtime_r(sec, &tm);
#else
	tm = *gmtime(sec); // NOLINT(runtime/threadsafe_fn)
#endif
	return tm;
}

int _yΩIO_print_tm(yπio_printctx_t *t) {
	const struct tm tm = yπio_printctx_va_arg(t, struct tm);
	return _yΩIO_print_time_strftime(t, &tm);
}


#if _yIO_HAS_timespec
int _yΩIO_print_timespec(yπio_printctx_t *t) {
	const struct timespec ts = yπio_printctx_va_arg(t, struct timespec);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, TC("{}.{:09}"), ts.tv_sec, ts.tv_nsec);
}
#endif // _yIO_HAS_timespec

#if _yIO_HAS_timeval
int _yΩIO_print_timeval(yπio_printctx_t *t) {
	const struct timeval ts = yπio_printctx_va_arg(t, struct timeval);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, TC("{}.{:06}"), ts.tv_sec, ts.tv_usec);
}
#endif // _yIO_HAS_timeval

