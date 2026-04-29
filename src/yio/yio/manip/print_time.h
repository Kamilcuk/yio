/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_TIME_H_
#define YYIO_YIO_YIO_MANIP_PRINT_TIME_H_
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
int YYIO_print_time_in_extract_format_add_space(char *dest, const char *fmt, const char **enptr);

int YYIO_print_tm(yio_printctx_t *t);
int YYIO_print_tm_pointer(yio_printctx_t *t);
int YYIO_print_localtime(yio_printctx_t *t);
int YYIO_print_gmtime(yio_printctx_t *t);

/**
 * @define yio_localtime(v)
 * @param v time_t value
 * Calls localtime() on v and prints it.
 */
#define yio_localtime(v)  yio_callback(YYIO_print_localtime, (time_t)(v))
/**
 * @define yio_gmtime(v)
 * @param v time_t value
 * Calls gmtime() on v and prints it.
 */
#define yio_gmtime(v)     yio_callback(YYIO_print_gmtime, (time_t)(v))

#if YYIO_HAS_timespec
int YYIO_print_timespec(yio_printctx_t *t);
#define YYIO_PRINT_GENERIC_TIMESPEC() \
		YYIO_OVERLOAD_TYPE_FUNC(struct timespec, YYIO_print_timespec)
#else
#define YYIO_PRINT_GENERIC_TIMESPEC()
#endif

#if YYIO_HAS_timeval
int YYIO_print_timeval(yio_printctx_t *t);
#define YYIO_PRINT_GENERIC_TIMEVAL() \
		YYIO_OVERLOAD_TYPE_FUNC(struct timeval, YYIO_print_timeval)
#else // YYIO_HAS_timeval
#define YYIO_PRINT_GENERIC_TIMEVAL()
#endif // YYIO_HAS_timeval

#define YYIO_PRINT_GENERIC_TIME() \
		YYIO_OVERLOAD_TYPE_FUNC(struct tm, YYIO_print_tm) \
		YYIO_OVERLOAD_POINTER_TYPE_FUNC(struct tm*, YYIO_print_tm_pointer) \
		YYIO_PRINT_GENERIC_TIMESPEC() \
		YYIO_PRINT_GENERIC_TIMEVAL()

#endif /* YYIO_YIO_YIO_MANIP_PRINT_TIME_H_ */
