/**
 * @file
 * @date 2020-sie-01
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"


static inline _yIO_access_r(2, 3) _yIO_wur _yIO_nn()
size_t _yΩIO_fwrite(FILE *file, const Ychar* str, size_t size) {
	{% if MODEX == 1 %}
	#line
	return fwrite(str, 1, size, file);
	{% elif MODEX == 2 %}
	#line
	for (size_t n = 0; n < size; n++) {
		if (fputwc(str[n], file) == YEOF) {
			return n;
		}
	}
	return size;
	{% else %}
	#line
	const bool isnormal = fwide(file, 0) <= 0;
	if (isnormal) {
		const char *mb; size_t mb_len;
		int ret = _yIO_strconv_πstr_to_str(str, size, &mb, &mb_len);
		if (ret) return ret;
		size_t r = fwrite(mb, 1, mb_len, file);
		_yIO_strconv_free_πstr_to_str(str, mb);
		if (r != mb_len) return -1;
	} else {
		const wchar_t *wc; size_t wc_len;
		int ret = _yIO_strconv_πstr_to_wstr(str, size, &wc, &wc_len);
		if (ret) return ret;
		for (size_t i = wc_len; i--; ) {
			if (fputwc(wc[i], file) == YEOF) {
				_yIO_strconv_free_πstr_to_wstr(str, wc);
				return i;
			}
		}
		_yIO_strconv_free_πstr_to_wstr(str, wc);
	}
	return size;
	{% endif %}
}

static _yIO_access_r(2, 3) _yIO_wur _yIO_nn()
int _yΩIO_yvfprintf_cb(void *arg, const Ychar *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = _yΩIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}


int _yΩIO_yfprintf(FILE *file, yπio_printdata_t *data, const Ychar *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int yπvfprintf(FILE *file, yπio_printdata_t *data, const Ychar *fmt, va_list *va) {
	return yπvbprintf(_yΩIO_yvfprintf_cb, file, data, fmt, va);
}

