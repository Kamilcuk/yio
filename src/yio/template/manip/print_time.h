/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef _yIO_YIO_YΩIO_MANIP_PRINT_TIME_H_
#define _yIO_YIO_YΩIO_MANIP_PRINT_TIME_H_
#include <time.h>
#ifndef _yIO_HAS_timespec
#error
#endif
#ifndef _yIO_HAS_timeval
#error
#endif
#if _yIO_HAS_timespec || _yIO_HAS_timeval
#include <sys/time.h>
#endif

/**
 * Copies the formatting string that will be passed to strftime
 * in such way that \{\{ and \}\} sequences are transformed into \{ and \}.
 * A freestanding } ends the formatting string.
 * A freestanding { is a formatting error.
 * @param dest Either a NULL or a valid pointer to allocated memory enough
 * to hold the result.
 * @param fmt
 * @return Negative on error. Otherwise returns the number of bytes excluding
 * zero terminating character that would have been written to or were written to
 * @c dest depending if it's a null pointer or not.
 */
int _yΩIO_print_time_in_extract_format_add_space(TCHAR *dest, const TCHAR *fmt, const TCHAR **enptr);

int _yΩIO_print_tm(yπio_printctx_t *t);

int _yΩIO_print_time_gmtime(yπio_printctx_t *t);
#define yπptgmtime(time_t_variable) \
		yiocb(_Generic((time_t_variable), \
		time_t: _yΩIO_print_time_gmtime \
		),(time_t_variable))

int _yΩIO_print_time_localtime(yπio_printctx_t *t);
#define yπptlocaltime(time_t_variable) \
		yiocb(_Generic((time_t_variable), \
		time_t: _yΩIO_print_time_localtime \
		),(time_t_variable))

#if _yIO_HAS_timespec
int _yΩIO_print_timespec(yπio_printctx_t *t);
#define _yΩIO_PRINT_GENERIC_TIMESPEC() \
		struct timespec: _yΩIO_print_timespec,
#else
#define _yΩIO_PRINT_GENERIC_TIMESPEC()
#endif

#if _yIO_HAS_timeval
int _yΩIO_print_timeval(yπio_printctx_t *t);
#define _yΩIO_PRINT_GENERIC_TIMEVAL() \
		struct timeval: _yΩIO_print_timeval,
#else // _yIO_HAS_timeval
#define _yΩIO_PRINT_GENERIC_TIMEVAL()
#endif // _yIO_HAS_timeval

#define _yΩIO_PRINT_GENERIC_TIME() \
		struct tm: _yΩIO_print_tm, \
		_yΩIO_PRINT_GENERIC_TIMESPEC() \
		_yΩIO_PRINT_GENERIC_TIMEVAL()

#endif /* _yIO_YIO_YΩIO_MANIP_PRINT_TIME_H_ */
