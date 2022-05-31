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
size_t YYΩIO_fwrite(FILE *file, const TCHAR* str, size_t size) {
#if TMODE == 1
	return fwrite(str, 1, size, file);
#elif TMODE == 2
	for (size_t n = 0; n < size; n++) {
		if (fputwc(str[n], file) == TEOF) {
			return n;
		}
	}
	return size;
#else
	const bool isnormal = fwide(file, 0) <= 0;
	if (isnormal) {
		const char *mb; size_t mb_len;
		int ret = YYIO_strconv_πstr_to_str(str, size, &mb, &mb_len);
		if (ret) return ret;
		size_t r = fwrite(mb, 1, mb_len, file);
		YYIO_strconv_free_πstr_to_str(str, mb);
		if (r != mb_len) return -1;
	} else {
		const wchar_t *wc; size_t wc_len;
		int ret = YYIO_strconv_πstr_to_wstr(str, size, &wc, &wc_len);
		if (ret) return ret;
		for (size_t i = wc_len; i--; ) {
			if (fputwc(wc[i], file) == TEOF) {
				YYIO_strconv_free_πstr_to_wstr(str, wc);
				return i;
			}
		}
		YYIO_strconv_free_πstr_to_wstr(str, wc);
	}
	return size;
#endif
}

static YYIO_access_r(2, 3) YYIO_wur YYIO_nn()
int YYΩIO_yvfprintf_cb(void *arg, const TCHAR *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = YYΩIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}


int YYΩIO_yfprintf(FILE *file, const yπio_printdata_t *data, const TCHAR *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int yπvfprintf(FILE *file, const yπio_printdata_t *data, const TCHAR *fmt, va_list *va) {
	return yπvbprintf(YYΩIO_yvfprintf_cb, file, data, fmt, va);
}

