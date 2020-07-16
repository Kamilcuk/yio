/**
 * @file
 * @date: 2020-lip-01
 * @author: Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config_yio_template(`m4_dnl);
#pragma once
#include <time.h>

/**
 * Copies the formatting string that will be passed to strftime
 * in such way that {{ and }} sequences are transformed into { and }.
 * A freestanding } ends the formatting string.
 * A freestanding { is a formatting error.
 * @param dest Either a NULL or a valid pointer to allocated memory enough
 * to hold the result.
 * @param fmt
 * @return -1 on error. Otherwise returns the number of bytes excluding
 * zero terminating character that would have been written to or were written to
 * @c dest depending if it's a null pointer or not.
 */
int _yIO_print_time_in_extract_format_add_space(char *dest, const char *fmt);

m4_define_function(`m4_print_time_gen1~,m4_dnl;
int $1(yπio_printctx_t *t);
int $1_pnt(yπio_printctx_t *t);
int $1_constpnt(yπio_printctx_t *t);
) m4_dnl ;

m4_define_function(`m4_print_time_gen2~,`m4_dnl;
		$1: $2, \
		$1 *: $2_pnt, \
		const $1 *: $2_constpnt, m4_dnl;
~) m4_dnl ;

m4_print_time_gen1(_yIO_print_tm)
#define _yIO_PRINT_GENERIC_TM() \
		m4_print_time_gen2(struct tm, _yIO_print_tm)

m4_print_time_gen1(_yIO_print_time_gmtime)
#define yptgmtime(time_t_variable) \
		yio(_Generic((time_t_variable), \
		m4_print_time_gen2(struct tm, _yIO_print_time_gmtime) \
		),(time_t_variable))

m4_print_time_gen1(_yIO_print_time_localtime)
#define yptlocaltime(time_t_variable) \
		_Generic((time_t_variable), \
		m4_print_time_gen2(struct tm, _yIO_print_time_localtime) \
		),(time_t_variable))

#ifdef _yIO_HAS_timespec
m4_print_time_gen1(_yIO_print_timespec)
#define _yIO_PRINT_GENERIC_TIMESPEC() \
		m4_print_time_gen2(struct timespec, _yIO_print_timespec)
#else // _yIO_HAS_timespec
#define _yIO_PRINT_GENERIC_TIMESPEC()
#endif // _yIO_HAS_timespec

#ifndef _yIO_HAS_timeval
m4_print_time_gen1(_yIO_print_timeval)
#define _yIO_PRINT_GENERIC_TIMEVAL() \
		m4_print_time_gen2(struct timeval, _yIO_print_timeval)
#else // _yIO_HAS_timeval
#define _yIO_PRINT_GENERIC_TIMEVAL()
#endif // _yIO_HAS_timeval

#define _yIO_PRINT_GENERIC_TIME() \
		_yIO_PRINT_GENERIC_TM() \
		_yIO_PRINT_GENERIC_TIMESPEC() \
		_yIO_PRINT_GENERIC_TIMEVAL()

~)m4_dnl m4_config_yio_template ;
