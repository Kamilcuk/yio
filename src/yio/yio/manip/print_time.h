/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YΩIO_MANIP_PRINT_TIME_H_
#define YYIO_YIO_YΩIO_MANIP_PRINT_TIME_H_
#include "../ctx.h"
#include <time.h>
#ifndef YYIO_HAS_timespec
#error
#endif
#ifndef YYIO_HAS_timeval
#error
#endif
#if YYIO_HAS_timespec || YYIO_HAS_timeval
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
int YYΩIO_print_time_in_extract_format_add_space(TCHAR *dest, const TCHAR *fmt, const TCHAR **enptr);

int YYΩIO_print_tm(yπio_printctx_t *t);

#if YYIO_HAS_timespec
int YYΩIO_print_timespec(yπio_printctx_t *t);
#define YYΩIO_PRINT_GENERIC_TIMESPEC() \
		struct timespec: YYΩIO_print_timespec,
#else
#define YYΩIO_PRINT_GENERIC_TIMESPEC()
#endif

#if YYIO_HAS_timeval
int YYΩIO_print_timeval(yπio_printctx_t *t);
#define YYΩIO_PRINT_GENERIC_TIMEVAL() \
		struct timeval: YYΩIO_print_timeval,
#else // YYIO_HAS_timeval
#define YYΩIO_PRINT_GENERIC_TIMEVAL()
#endif // YYIO_HAS_timeval

#define YYΩIO_PRINT_GENERIC_TIME() \
		struct tm: YYΩIO_print_tm, \
		YYΩIO_PRINT_GENERIC_TIMESPEC() \
		YYΩIO_PRINT_GENERIC_TIMEVAL()

#endif /* YYIO_YIO_YΩIO_MANIP_PRINT_TIME_H_ */
