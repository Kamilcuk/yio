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
#ifndef YIO_HAS_MONETARY_H
#error
#endif
#if YIO_HAS_MONETARY_H
#include <monetary.h>
#endif

static const size_t MAXSIZE = 4096U;

int YIO_astrftime_nonzero(YIO_buf *res, const char *fmt, const struct tm *tm) {
#ifdef __SDCC
	(void)res; (void)fmt; (void)tm;
	return YIO_ERROR_ENOSYS;
#else
	while (1) {
		const size_t bufsize = YIO_buf_capacity(res);
		errno = 0;
		size_t count = strftime(YIO_buf_data(res), bufsize, fmt, tm);
		// dbgln("%zu %d %s %s %zu", count, errno, strerror(errno), fmt, YIO_buf_capacity(res));
		if (count != (size_t)0) {
			YIO_buf_set_used(res, count);
			break;
		}
		// MUSL set's EINVAL when buffer is too small
		if (errno != 0 && errno != EINVAL) {
			return YIO_ERROR(YIO_ERROR_STRFTIME, "strftime returned -1 and errno is set");
		}
		if (bufsize > MAXSIZE) {
			return YIO_ERROR(YIO_ERROR_STRFTIME_TOOBIG, "strftime needed more than 4096 bytes to write");
		}
		int err = YIO_buf_reserve_more(res, 0);
		if (err) return err;
	}
	return 0;
#endif
}

#if YIO_HAS_MONETARY_H
int YIO_astrfmon(YIO_buf *res, const char *fmt, struct YIO_astrfmon_arg arg) {
	while (1) {
		char *const buf = YIO_buf_data(res);
		const size_t bufsize = YIO_buf_capacity(res);
		errno = 0;
		const ssize_t count =
#if YIO_HAS_FLOATl
			arg.isldbl ?
			strfmon(buf, bufsize, fmt, arg.v.ld) :
#endif
			strfmon(buf, bufsize, fmt, arg.v.d);
		//dbgln("bufsize=%d count=%d errno=%d buf=`%.*s`", (int)bufsize, (int)count, errno, (int)bufsize, buf);
		if (count <= 0) {
			if (errno != 0 && errno != E2BIG) {
				//dbgln("%d %d %d %s", (int)count, (int)bufsize, errno, strerror(errno));
				// GLIBC returns -1 and set's errno to 0
				// https://sourceware.org/bugzilla/show_bug.cgi?id=29090
				return YIO_ERROR(YIO_ERROR_STRFMON, "strfmon returned -1 and errno is set and it's not E2BIG");
			}
		} else {
			// We explicitly wait for less characters than buffer size.
			// Musl does that.
			if (count < (ssize_t)bufsize) {
				//dbgln("%d %d %d %s", (int)count, (int)bufsize, errno, strerror(errno));
				YIO_buf_set_used(res, count);
				break;
			}
		}
		if (bufsize > MAXSIZE) {
			return YIO_ERROR(YIO_ERROR_STRFMON_TOOBIG, "strfmon needed more than 4096 bytes to write");
		}
		int err = YIO_buf_reserve_more(res, 0);
		if (err) return err;
	}
	return 0;
}
#endif

