/**
 * @file
 * @date 05/04/2020
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#ifndef _yIO_HAS_UNISTD_H
#error
#endif
#if _yIO_HAS_UNISTD_H
#include <unistd.h>
#include <errno.h>

static inline _yIO_access_r(2, 3)
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

#if TMODEX == 2 && defined __NEWLIB__ && defined _FORTIFY_SOURCE
		// there is a bug in newlib
		// in include/ssp/wchar.h when checking size for wcrtomb
#define SUPER_MB_LEN_MAX  (MB_LEN_MAX > sizeof(wchar_t) ? MB_LEN_MAX :  sizeof(wchar_t))
#else
#define SUPER_MB_LEN_MAX  (MB_LEN_MAX)
#endif

static inline _yIO_access_r(2, 3)
int _yΩIO_yπvdprintf_cb(void *arg, const TCHAR *ptr, size_t size) {
#if TMODE == 1
	return _yΩIO_yπvdprintf_cb_in(arg, ptr, size);
#elif TMODE == 2 || TMODE == 3 || TMODE == 4
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	while (size--) {
		char s[SUPER_MB_LEN_MAX];
		const size_t wr =
#if TMODE == 2
			wcrtomb(s, *ptr, &ps)
#elif TMODE == 3
			c16rtomb(s, *ptr, &ps)
#elif TMODE == 4
			c32rtomb(s, *ptr, &ps)
#else
#error
#endif
		;
		ptr++;
		if (wr == (size_t)-1) {
			return (TMODE == 2) ? YIO_ERROR_WCTOMB : (TMODE == 3) ? YIO_ERROR_C16TOMB : YIO_ERROR_C32TOMB;
		}
		const int r = _yΩIO_yπvdprintf_cb_in(arg, s, wr);
		if (r < 0) return r;
	}
	return 0;
#else
#error
#endif
}

int yπvdprintf(int fd, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va) {
	return yπvbprintf(_yΩIO_yπvdprintf_cb, &fd, data, fmt, va);
}

int _yΩIO_ydprintf(int fd, const yπio_printdata_t *data, const TCHAR *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvdprintf(fd, data, fmt, &va);
	va_end(va);
	return ret;
}

#endif // _yIO_HAS_UNISTD
