/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
#define YIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

#ifndef YIO_HAS_timespec
#error
#endif
#ifndef YIO_HAS_timeval
#error
#endif
#if (YIO_HAS_timespec || YIO_HAS_timeval) && YIO_HAS_SYS_TIME_H
#include <sys/time.h>
#endif

#if YIO_HAS_timespec || !defined(__SDCC)
#include <time.h>
int YIO_print_timespec(yio_printctx_t *t);
#define YIO_PRINT_GENERIC_TIMESPEC(X, XALIAS) \
		X(struct timespec, YIO_print_timespec)
#else
#define YIO_PRINT_GENERIC_TIMESPEC(X, XALIAS)
#endif

#if YIO_HAS_timeval || !defined(__SDCC)
int YIO_print_timeval(yio_printctx_t *t);
#define YIO_PRINT_GENERIC_TIMEVAL(X, XALIAS) \
		X(struct timeval, YIO_print_timeval)
#else // YIO_HAS_timeval
#define YIO_PRINT_GENERIC_TIMEVAL(X, XALIAS)
#endif // YIO_HAS_timeval


#ifdef __cplusplus
}
#endif
#endif // YIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
