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

#if {{(MODEX == 2)|int}} && defined __NEWLIB__ && defined _FORTIFY_SOURCE
		// there is a bug in newlib
		// in include/ssp/wchar.h when checking size for wcrtomb
#define SUPER_MB_LEN_MAX  (MB_LEN_MAX > sizeof(wchar_t) ? MB_LEN_MAX :  sizeof(wchar_t))
#else
#define SUPER_MB_LEN_MAX  (MB_LEN_MAX)
#endif

static inline _yIO_access_r(2, 3)
int _yΩIO_yπvdprintf_cb(void *arg, const Ychar *ptr, size_t size) {
{% if MODEX == 1 %}
#line
	return _yΩIO_yπvdprintf_cb_in(arg, ptr, size);
{% elif MODEX == 2 or MODEX == 3 %}
#line
	mbstate_t ps;
	memset(&ps, 0, sizeof(ps));
	while (size--) {
		char s[SUPER_MB_LEN_MAX];
		const size_t wr =
				{% if MODE == 2 %} wcrtomb {% elif MODE == 3 %} c16rtomb {% elif MODE == 4 %} c32rtomb {% else %}{{ j_fatal() }}{% endif %}
				(s, *ptr, &ps);
#line
		ptr++;
		if (wr == (size_t)-1) return YIO_ERROR_WCTOMB;
		const int r = _yΩIO_yπvdprintf_cb_in(arg, s, wr);
		if (r < 0) return r;
	}
	return 0;
{% else %}{{ j_fatal() }}{% endif %}
}

int yπvdprintf(int fd, const yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	return yπvbprintf(_yΩIO_yπvdprintf_cb, &fd, data, fmt, va);
}

int _yΩIO_ydprintf(int fd, const yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvdprintf(fd, data, fmt, &va);
	va_end(va);
	return ret;
}

#endif // _yIO_HAS_UNISTD
