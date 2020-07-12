/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config(yio);
#include "private.h"
#ifdef _yIO_HAS_UNISTD_H
#include <unistd.h>
#include <errno.h>

int _yIO_yvdprintf_cb(void *arg, const Ychar *ptr0, size_t size) {
	const int fd = *(int*)arg;
	const char *ptr = (const char *)ptr0;
	int ret = 0;
	while (size) {
		const ssize_t written = write(fd, ptr, size);
		if (ret < 0) {
			ret = EIO;
			break;
		}
		size -= written;
		ptr += written;
	}
	return ret;
}

int yvdprintf(int fd, yio_printdata_t *data, va_list *va) {
	return yvbprintf(_yIO_yvdprintf_cb, &fd, data, va);
}

int _yIO_ydprintf(int fd, const struct _yIO_printdata_s *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yvdprintf(fd, data, &va);
	va_end(va);
	return ret;
}

/* ---------------------------------------------------------------- */

__attribute__((__access__(__write_only__, 2, 3)))
static inline
int _yIO_ydscanf_cb_read_repeat(int fd, char *dest, size_t size) {
	int ret = 0;
	while (size) {
		const ssize_t readed = read(fd, dest, size);
		if (ret < 0) {
			if (errno == EAGAIN) {
				ret = -1;
			} else {
				ret = EIO;
			}
			break;
		}
		size -= readed;
		dest += readed;
	}
	return ret;
}

static
int _yIO_ydscanf_cb(void *arg, Yint *data) {
	const int fd = *(int*)arg;
	Ychar c = 0;
	int ret = _yIO_ydscanf_cb_read_repeat(fd, &c, sizeof(c));
	if (ret == -1) {
		*data = YEOF;
		ret = 0;
	} else if (ret == 0) {
		*data = c;
	}
	return ret;
}

struct yio_scanret_s yvdscanf(int fd, yio_scandata_t *data, va_list *va) {
	return yvbscanf(&_yIO_ydscanf_cb, &fd, data, va);
}

struct yio_scanret_s _yIO_ydscanf(int fd, yio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yio_scanret_s ret = yvdscanf(fd, data, &va);
	va_end(va);
	return ret;
}

#endif // _yIO_HAS_UNISTD
