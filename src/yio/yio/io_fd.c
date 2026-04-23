/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#ifndef YYIO_HAS_UNISTD_H
#error
#endif
#if YYIO_HAS_UNISTD_H
#include <unistd.h>
#include <errno.h>

static inline YYIO_access_r(2, 3)
int YYIO_yvdprintf_cb_in(void *arg, const char *ptr, size_t size) {
	const int fd = *(int*)arg;
	int ret = 0;
	while (size) {
		const ssize_t written = write(fd, ptr, size);
		if (ret < 0) {
			if (errno == EAGAIN) {
				continue;
			}
			ret = EIO;
			break;
		}
		size -= written;
		ptr += written;
	}
	return ret;
}

static inline YYIO_access_r(2, 3)
int YYIO_yvdprintf_cb(void *arg, const char *ptr, size_t size) {
	return YYIO_yvdprintf_cb_in(arg, ptr, size);
}

int yvdprintf(int fd, const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yvbprintf(YYIO_yvdprintf_cb, &fd, data, fmt, va);
}

int YYIO_ydprintf(int fd, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvdprintf(fd, data, fmt, &va);
	va_end(va);
	return ret;
}

#endif // YYIO_HAS_UNISTD
