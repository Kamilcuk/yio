/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#ifndef YYIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
#define YYIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../ctx_types.h"

#ifndef YYIO_HAS_timespec
#error
#endif
#ifndef YYIO_HAS_timeval
#error
#endif
#if (YYIO_HAS_timespec || YYIO_HAS_timeval) && YYIO_HAS_SYS_TIME_H
#include <sys/time.h>
#endif

#if YYIO_HAS_timespec || !defined(__SDCC)
#include <time.h>
int YYIO_print_timespec(yio_printctx_t *t);
#define YYIO_PRINT_GENERIC_TIMESPEC(X, XALIAS) \
		X(struct timespec, YYIO_print_timespec)
#else
#define YYIO_PRINT_GENERIC_TIMESPEC(X, XALIAS)
#endif

#if YYIO_HAS_timeval || !defined(__SDCC)
int YYIO_print_timeval(yio_printctx_t *t);
#define YYIO_PRINT_GENERIC_TIMEVAL(X, XALIAS) \
		X(struct timeval, YYIO_print_timeval)
#else // YYIO_HAS_timeval
#define YYIO_PRINT_GENERIC_TIMEVAL(X, XALIAS)
#endif // YYIO_HAS_timeval


#ifdef __cplusplus
}
#endif
#endif // YYIO_YIO_YIO_MANIP_PRINT_TIMEVALSPEC_H_
