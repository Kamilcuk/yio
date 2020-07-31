/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#ifdef _yIO_HAS_UNISTD_H
#include <unistd.h>
#include <errno.h>

static inline _yIO_access(__read_only__, 2, 3)
int _yΩIO_yπvdprintf_cb_in(void *arg, const char *ptr, size_t size) {
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

static inline _yIO_access(__read_only__, 2, 3)
int _yΩIO_yπvdprintf_cb(void *arg, const Ychar *ptr, size_t size) {
#if _yIO_TYPE_YIO
	return _yΩIO_yπvdprintf_cb_in(arg, ptr, size);
#elif _yIO_TYPE_YWIO
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	while (size--) {
		char s[MB_LEN_MAX];
		const size_t wr = wcrtomb(s, *ptr++, &ps);
		if (wr == (size_t)-1) return YIO_ERROR_WCTOMB_ERR;
		const int r = _yΩIO_yπvdprintf_cb_in(arg, s, wr);
		if (r < 0) return r;
	}
	return 0;
#elif _yIO_TYPE_YUIO
	size_t length = 0;
	const char * const string = u32_conv_to_encoding(locale_charset(),
			iconveh_question_mark, ptr, size, NULL, NULL, &length);
	if (string == NULL) return YIO_ERROR_U32_CONV_TO_ENCODING;
	const int r = _yΩIO_yπvdprintf_cb_in(arg, string, length);
	if (r < 0) return r;
	free((void*)string);
	return 0;
#else
#error
#endif
}

int yπvdprintf(int fd, yπio_printdata_t *data, va_list *va) {
	return yπvbprintf(_yΩIO_yπvdprintf_cb, &fd, data, va);
}

int _yΩIO_ydprintf(int fd, yπio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yπvdprintf(fd, data, &va);
	va_end(va);
	return ret;
}

/* ---------------------------------------------------------------- */

static inline _yIO_access(__write_only__, 2, 3)
int _yΩIO_ydscanf_cb_read_repeat(int fd, void *dest0, size_t size) {
	char *dest = dest0;
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
int _yΩIO_ydscanf_cb(void *arg, Yint *data) {
	const int fd = *(int*)arg;
	Ychar c = 0;
	int ret = _yΩIO_ydscanf_cb_read_repeat(fd, &c, sizeof(c));
	if (ret == -1) {
		*data = YEOF;
		ret = 0;
	} else if (ret == 0) {
		*data = c;
	}
	return ret;
}

struct yπio_scanret_s yπvdscanf(int fd, yπio_scandata_t *data, va_list *va) {
	return yπvbscanf(&_yΩIO_ydscanf_cb, &fd, data, va);
}

struct yπio_scanret_s _yΩIO_ydscanf(int fd, yπio_scandata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const struct yπio_scanret_s ret = yπvdscanf(fd, data, &va);
	va_end(va);
	return ret;
}

#endif // _yIO_HAS_UNISTD
