/**
 * @file
 * @date 2020-sie-01
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"

static inline YYIO_access_r(2, 3) YYIO_wur YYIO_nn()
size_t YYIO_fwrite(FILE *file, const char* str, size_t size) {
#if 1 == 1
	return fwrite(str, 1, size, file);
#elif 1 == 2
	for (size_t n = 0; n < size; n++) {
		if (fputwc(str[n], file) == EOF) {
			return n;
		}
	}
	return size;
#elif 1 == 3 || 1 == 4
	const bool isnormal = fwide(file, 0) <= 0;
	if (isnormal) {
		const char *mb; size_t mb_len;
		int ret = YYIO_strconv_str_to_str(str, size, &mb, &mb_len);
		if (ret) return ret;
		size_t r = fwrite(mb, 1, mb_len, file);
		YYIO_strconv_free_str_to_str(str, mb);
		if (r != mb_len) return -1;
	} else {
		const wchar_t *wc; size_t wc_len;
		int ret = YYIO_strconv_str_to_wstr(str, size, &wc, &wc_len);
		if (ret) return ret;
		for (size_t i = wc_len; i--; ) {
			if (fputwc(wc[i], file) == EOF) {
				YYIO_strconv_free_str_to_wstr(str, wc);
				return i;
			}
		}
		YYIO_strconv_free_str_to_wstr(str, wc);
	}
	return size;
#else
#error
#endif
}

static YYIO_access_r(2, 3) YYIO_wur YYIO_nn()
int YYIO_yvfprintf_cb(void *arg, const char *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = YYIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}


int YYIO_yfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int yvfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yvbprintf(YYIO_yvfprintf_cb, file, data, fmt, va);
}

