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
#include "yio/private/yio_time.h"
#include <time.h>
#if _yIO_HAS_UNISTD_H
#include <sys/time.h>
#endif

int _yΩIO_print_time_in_extract_format_add_space(Ychar *dest, const Ychar *fmt, const Ychar **endptr) {
	int ret = 0;
	if (fmt) {
		while (fmt[0] != Yc('\0')) {
			if (fmt[0] == '{' || fmt[0] == Yc('}')) {
				if (fmt[1] != fmt[0]) {
					if (fmt[0] == Yc('{')) {
						ret = -1;
						goto EXIT;
					}
					// fmt[0] == '}'
					break;
				}

				if (dest) *dest++ = fmt[0];
				ret++;
				fmt+=2;
			} else {
				if (dest) *dest++ = fmt[0];
				ret++;
				fmt++;
			}
		}
		if (fmt[0] != Yc('}')) {
			ret = -2;
			goto EXIT;
		}
	}
	// empty string results in %c
	if (ret == 0) {
		if (dest) {
			*dest++ = Yc('%');
			*dest++ = Yc('c');
		}
		ret = 2;
	}
	if (dest) {
		// Add additional special space so that strftime
		// never returns zero.
		*dest++ = Yc(' ');
		*dest = Yc('\0');
	}
	// Return +1 for additional space.
	ret += 1;
	//
	EXIT:
	if (endptr && fmt) {
		*endptr = fmt + 1;
	}
	return ret;
}

static inline
int _yΩIO_print_time_strftime(yπio_printctx_t *t, const struct tm *tm) {
	int ret = 0;

	int len = _yΩIO_print_time_in_extract_format_add_space(NULL, t->fmt, NULL);
	if (len < 0) {
		ret = YIO_ERROR_PYFMT_INVALID;
		goto FORMAT_EXTRACT_ERROR;
	}
	Ychar *format_in = malloc(sizeof(*format_in) * (len + 1));
	if (format_in == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto FORMAT_MALLOC_ERROR;
	}
	const int len2 = _yΩIO_print_time_in_extract_format_add_space(format_in, t->fmt, &t->fmt);
	(void)len2; assert(len2 == len);

	const char *format;
	{% if MODEX == 1 %}
#line
	format_in[len] = '\0';
	format = format_in;
	if (0) { // silence warning
		goto FORMAT_STRCONV_ERROR;
	}
	{% else %}
#line
	ret = _yIO_strconv_πstr_to_str(format_in, len, &format, NULL);
	if (ret) {
		ret = YIO_ERROR_ENOMEM;
		goto FORMAT_STRCONV_ERROR;
	}

	char *pnt = realloc((void*)format, sizeof(*format) * (len + 1));
	if (pnt == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto FORMAT_STRCONV_ERROR;
	}
	pnt[len] = '\0';
	format = pnt;
	{% endif %}
#line

	// 80 is somewhat a psuedo standard here
	char _buf_mem[80];
	char *buf = _buf_mem;
	int length = strftime(buf, sizeof(_buf_mem), format, tm);
	if (length == 0) {
		// if we fail, allocate dynamically
		length = _yIO_astrftime_nonzero(&buf, sizeof(_buf_mem) * 2, format, tm);
	}
	if (length <= 0) {
		ret = YIO_ERROR_ENOMEM;
		goto STRFTIME_ERROR;
	}

	// We pass length - 1 cause without extra space we allocated in format string.
	// Note that this _can_ result in an empty string here.
	ret = yπio_printctx_put(t, buf, length - 1);

	if (buf != _buf_mem) {
		free(buf);
	}
	STRFTIME_ERROR:
	_yIO_strconv_free_πstr_to_str(format_in, format);
	FORMAT_STRCONV_ERROR:
	free(format_in);
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

/* ------------------------------------------------------------------------- */

int _yΩIO_print_time_localtime(yπio_printctx_t *t) {
	const time_t ts = yπio_printctx_va_arg(t, time_t);
	const struct tm tm = _yIO_localtime(&ts);
	return _yΩIO_print_time_strftime(t, &tm);
}

int _yΩIO_print_time_gmtime(yπio_printctx_t *t) {
	const time_t ts = yπio_printctx_va_arg(t, time_t);
	const struct tm tm = _yIO_gmtime(&ts);
	return _yΩIO_print_time_strftime(t, &tm);
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
	return yπio_printctx_printf(t, Yc("{}.{:09}"), ts.tv_sec, ts.tv_nsec);
}
#endif // _yIO_HAS_timespec

#if _yIO_HAS_timeval
int _yΩIO_print_timeval(yπio_printctx_t *t) {
	const struct timeval ts = yπio_printctx_va_arg(t, struct timeval);
	int err = yπio_printctx_init(t);
	if (err) return err;
	return yπio_printctx_printf(t, Yc("{}.{:06}"), ts.tv_sec, ts.tv_usec);
}
#endif // _yIO_HAS_timeval

