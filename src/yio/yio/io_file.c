/**
 * @file
 * @date 2020-sie-01
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"

#ifndef YYIO_HAS_FILE
#error YYIO_HAS_FILE is not defined
#endif

#if YYIO_HAS_FILE

static inline YYIO_access_r(2, 3) YYIO_wur YYIO_nn()
size_t YYIO_fwrite(FILE *file, const char* str, size_t size) {
	return fwrite(str, 1, size, file);
}

static YYIO_access_r(2, 3) YYIO_wur YYIO_nn()
int YYIO_yio_vfprintf_cb(void *arg, const char *ptr, size_t size) {
	FILE *f = arg;
	const size_t cnt = YYIO_fwrite(f, ptr, size);
	return cnt == size ? 0 : YIO_ERROR_EIO;
}


int YYIO_yio_fprintf(FILE *file, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vfprintf(file, data, fmt, &va);
	va_end(va);
	return ret;
}

int yio_vfprintf(FILE *file, const yio_printdata_t *data, const char *fmt, va_list *va) {
	return yio_vbprintf(YYIO_yio_vfprintf_cb, file, data, fmt, va);
}

#endif // YYIO_HAS_FILE

