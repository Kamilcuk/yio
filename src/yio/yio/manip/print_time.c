/**
 * @file
 * @date: 2020-lip-02
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
#include "private.h"
#include "yio/private/yio_time.h"
#include <time.h>
#ifdef _yIO_HAS_UNISTD_H
#include <sys/time.h>
#endif

int _yIO_print_time_in_extract_format_add_space(char *dest, const char *fmt) {
	int ret = 0;
	while (fmt[0] != '\0') {
		if (fmt[0] == '{' || fmt[0] == '}') {
			if (fmt[1] != fmt[0]) {
				if (fmt[0] == '{') {
					return -1;
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
	if (fmt[0] != '}') {
		return -1;
	}
	// empty string results in %c
	if (ret == 0) {
		if (dest) strcpy(dest, "%c ");
		return 3;
	}
	if (dest) {
		// Add additional special space so that strftime
		// never returns zero.
		*dest++ = ' ';
		*dest = '\0';
	}
	// Return +1 for additional space.
	return ret + 1;
}

static inline
int _yIO_print_time_strftime(yπio_printctx_t *t, const struct tm *tm) {
	int ret = 0;

	const char *format_string = t->fmt;
	int len = _yIO_print_time_in_extract_format_add_space(NULL, format_string);
	if (len < 0) {
		ret = YIO_ERROR_PYFMT_INVALID;
		goto FORMAT_EXTRACT_ERROR;
	}
	char *format = malloc(len + 1);
	if (format == NULL) {
		ret = YIO_ERROR_ENOMEM;
		goto FORMAT_MALLOC_ERROR;
	}
	const int len2 = _yIO_print_time_in_extract_format_add_space(format, format_string);
	(void)len2; assert(len2 == len);

	// 80 is somewhat a psuedo standard here
	char _buf_mem[80];
	char *buf = _buf_mem;
	int length = strftime(buf, sizeof(_buf_mem), format, tm);
	if (length == 0) {
		// if we fail, allocate dynamically
		length = _yIO_astrftime_nonzero(&buf, sizeof(_buf_mem), format, tm);
	}
	free(format);
	if (length <= 0) {
		ret = YIO_ERROR_ENOMEM;
		goto STRFTIME_ERROR;
	}

	// We pass length - 1 cause without extra space we allocated in format string.
	// Note that this _can_ result in an empty string here.
	ret = yπio_printctx_putsn(t, length - 1, buf);

	STRFTIME_ERROR:
	// If buf was dynamically allocated, free it.
	if (buf != _buf_mem) {
		free(buf);
	}
	FORMAT_MALLOC_ERROR:
	FORMAT_EXTRACT_ERROR:
	return ret;
}

static inline
struct tm _yIO_localtime(const time_t *sec) {
	struct tm tm;
#ifdef _yIO_HAS_localtime_r
	localtime_r(sec, &tm);
#else
	tm = *localtime(sec);
#endif
	return tm;
}

static inline
struct tm _yIO_gmtime(const time_t *sec) {
	struct tm tm;
#ifdef _yIO_HAS_localtime_r
	gmtime_r(sec, &tm);
#else
	tm = *gmtime(sec);
#endif
	return tm;
}

/* ----------------------------------------------------------------------------------------- */

// Generate the function for variable, pointer to and const pointer to variable
m4_define_function(`m4_print_time_gen3~, `m4_dnl;
int $2(yπio_printctx_t *t) {
	const $1 ts = yπio_printctx_va_arg(t, $1);
	return $2_in(t, &ts);
}
int $2_pnt(yπio_printctx_t *t) {
	const $1 *ts = yπio_printctx_va_arg(t, $1 *);
	return $2_in(t, ts);
}
int $2_constpnt(yπio_printctx_t *t) {
	const $1 *ts = yπio_printctx_va_arg(t, const $1 *);
	return $2_in(t, ts);
}
~) m4_dnl m4_define ;

/* time_t localtime ------------------------------------------------------ */

static inline
int _yIO_print_time_localtime_in(yπio_printctx_t *t, const time_t *sec) {
	const struct tm tm = _yIO_localtime(sec);
	return _yIO_print_time_strftime(t, &tm);
}

m4_print_time_gen3(time_t, _yIO_print_time_localtime)

/* time_t gmtime ------------------------------------------------------ */

static inline
int _yIO_print_time_gmtime_in(yπio_printctx_t *t, const time_t *sec) {
	const struct tm tm = _yIO_gmtime(sec);
	return _yIO_print_time_strftime(t, &tm);
}

m4_print_time_gen3(time_t, _yIO_print_time_gmtime)

/* tm ----------------------------------------------------------------------------------------- */

static inline
int _yIO_print_tm_in(yπio_printctx_t *t, const struct tm *tm) {
	return _yIO_print_time_strftime(t, tm);
}

m4_print_time_gen3(struct tm, _yIO_print_tm)

/* timespec ----------------------------------------------------------------------------------------- */

#ifdef _yIO_HAS_timespec

static inline
int _yIO_print_timespec_in(yπio_printctx_t *t, const struct timespec *ts) {
	return yπio_printctx_printf(t, "{}.{:09}", ts->tv_sec, ts->tv_nsec);
}

m4_print_time_gen3(struct timespec, _yIO_print_timespec)

#endif // _yIO_HAS_timespec

/* timeval ----------------------------------------------------------------------------------------- */

#ifdef _yIO_HAS_timeval

static inline
int _yIO_print_timeval_in(yπio_printctx_t *t, const struct timeval *ts) {
	return yπio_printctx_printf(t, "{}.{:06}", ts->tv_sec, ts->tv_usec);
}

m4_print_time_gen3(struct timeval, _yIO_print_timeval)

#endif // _yIO_HAS_timeval

~) m4_dnl m4_config_yio_template ;
