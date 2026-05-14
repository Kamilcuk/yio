/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#ifndef YIO_HAS_UNISTD_H
#error
#endif
#if YIO_HAS_UNISTD_H
#include <unistd.h>
#include <errno.h>

static inline YIO_access_r(2, 3)
int YIO_yio_vdprintf_cb_in(void *arg, const char *ptr, size_t size) {
	const int fd = *(int*)arg;
	int ret = 0;
	while (size) {
		const ssize_t written = write(fd, ptr, size);
		if (written < 0) {
			if (errno == EAGAIN) {
				continue;
			}
			ret = EIO;
			break;
		}
		size -= (size_t)written;
		ptr += written;
	}
	return ret;
}

static inline YIO_access_r(2, 3)
int YIO_yio_vdprintf_cb(void *arg, const char *ptr, size_t size) {
	return YIO_yio_vdprintf_cb_in(arg, ptr, size);
}

int yio_vdprintf(int fd, const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yio_vbprintf(YIO_yio_vdprintf_cb, &fd, data, fmt, va);
}

int YIO_yio_dprintf(int fd, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vdprintf(fd, data, fmt, &va);
	va_end(va);
	return ret;
}

#endif // YIO_HAS_UNISTD
