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
#ifndef _yIO_HAS_MONETARY_H
#error
#endif
#if _yIO_HAS_MONETARY_H
#include <monetary.h>
#endif

int _yIO_astrftime_nonzero(_yIO_res *res, const char *fmt, const struct tm *tm) {
	while (1) {
		const size_t bufsize = _yIO_res_size(res);
		errno = 0;
		size_t count = strftime(_yIO_res_begin(res), bufsize, fmt, tm);
		if (count != (size_t)0) {
			_yIO_res_set_used(res, count);
			break;
		}
		if (errno != 0) {
			return _yIO_ERROR(YIO_ERROR_STRFTIME, "strftime returned -1 and errno is set");
		}
		if (bufsize > 4096u) {
			return _yIO_ERROR(YIO_ERROR_STRFTIME_TOOBIG, "strftime needed more than 4096 bytes to write");
		}
		int err = _yIO_res_reserve_more(res);
		if (err) return err;
	}
	return 0;
}

#if _yIO_HAS_MONETARY_H
int _yIO_astrfmon(_yIO_res *res, const char *fmt, struct _yIO_astrfmon_arg arg) {
	while (1) {
		char *const buf = _yIO_res_begin(res);
		const size_t bufsize = _yIO_res_size(res);
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
				return _yIO_ERROR(YIO_ERROR_STRFMON, "strfmon returned -1 and errno is set and it's not E2BIG");
			}
		} else {
			// We explicitly wait for less characters than buffer size.
			// Musl does that.
			if (count < (ssize_t)bufsize) {
				//dbgln("%d %d %d %s", (int)count, (int)bufsize, errno, strerror(errno));
				_yIO_res_set_used(res, count);
				break;
			}
		}
		if (bufsize > 4096) {
			return _yIO_ERROR(YIO_ERROR_STRFMON_TOOBIG, "strfmon needed more than 4096 bytes to write");
		}
		int err = _yIO_res_reserve_more(res);
		if (err) return err;
	}
	return 0;
}
#endif

