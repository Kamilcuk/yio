/**
 * @file
 * @date 2020-sie-01
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"

#ifndef YIO_HAS_FILE
#error YIO_HAS_FILE is not defined
#endif

#if YIO_HAS_FILE

static inline YIO_access_r(2, 3) YIO_wur YIO_nn()
size_t YIO_fwrite(FILE *file, const char* str, size_t size) {
	return fwrite(str, 1, size, file);
}

static YIO_access_r(2, 3) YIO_wur YIO_nn()
int YIO_yio_vfprintf_cb(void *arg, const char *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = YIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}


int YIO_yio_fprint(FILE *file, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int yio_vfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yio_vbprintf(YIO_yio_vfprintf_cb, file, data, fmt, va);
}

#endif // YIO_HAS_FILE

