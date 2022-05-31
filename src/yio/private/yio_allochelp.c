/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_allochelp.h"
#include "private.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef YYIO_HAS_MONETARY_H
#error
#endif
#if YYIO_HAS_MONETARY_H
#include <monetary.h>
#endif

static const size_t MAXSIZE = 4096U;

int YYIO_astrftime_nonzero(YYIO_res *res, const char *fmt, const struct tm *tm) {
	while (1) {
		const size_t bufsize = YYIO_res_size(res);
		errno = 0;
		size_t count = strftime(YYIO_res_begin(res), bufsize, fmt, tm);
		// dbgln("%zu %d %s %s %zu", count, errno, strerror(errno), fmt, YYIO_res_size(res));
		if (count != (size_t)0) {
			YYIO_res_set_used(res, count);
			break;
		}
		// MUSL set's EINVAL when buffer is too small
		if (errno != 0 && errno != EINVAL) {
			return YYIO_ERROR(YIO_ERROR_STRFTIME, "strftime returned -1 and errno is set");
		}
		if (bufsize > MAXSIZE) {
			return YYIO_ERROR(YIO_ERROR_STRFTIME_TOOBIG, "strftime needed more than 4096 bytes to write");
		}
		int err = YYIO_res_reserve_more(res);
		if (err) return err;
	}
	return 0;
}

#if YYIO_HAS_MONETARY_H
int YYIO_astrfmon(YYIO_res *res, const char *fmt, struct YYIO_astrfmon_arg arg) {
	while (1) {
		char *const buf = YYIO_res_begin(res);
		const size_t bufsize = YYIO_res_size(res);
		errno = 0;
		const ssize_t count = arg.isldbl ?
			strfmon(buf, bufsize, fmt, arg.v.ld) :
			strfmon(buf, bufsize, fmt, arg.v.d);
		//dbgln("bufsize=%d count=%d errno=%d buf=`%.*s`", (int)bufsize, (int)count, errno, (int)bufsize, buf);
		if (count <= 0) {
			if (errno != 0 && errno != E2BIG) {
				//dbgln("%d %d %d %s", (int)count, (int)bufsize, errno, strerror(errno));
				// GLIBC returns -1 and set's errno to 0
				// https://sourceware.org/bugzilla/show_bug.cgi?id=29090
				return YYIO_ERROR(YIO_ERROR_STRFMON, "strfmon returned -1 and errno is set and it's not E2BIG");
			}
		} else {
			// We explicitly wait for less characters than buffer size.
			// Musl does that.
			if (count < (ssize_t)bufsize) {
				//dbgln("%d %d %d %s", (int)count, (int)bufsize, errno, strerror(errno));
				YYIO_res_set_used(res, count);
				break;
			}
		}
		if (bufsize > MAXSIZE) {
			return YYIO_ERROR(YIO_ERROR_STRFMON_TOOBIG, "strfmon needed more than 4096 bytes to write");
		}
		int err = YYIO_res_reserve_more(res);
		if (err) return err;
	}
	return 0;
}
#endif

