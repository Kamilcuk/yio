/**
 * @file yio_private.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_private.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static
void _yIO__test_failed_atexit(void) {
	_Exit(EXIT_FAILURE);
}

void _yIO__test_failed(const char *expr, const char *file, int line,
		const char *fmt, ...) {
	fprintf(stderr, "%s:%d: error: %s ", file, line, expr);
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
	fprintf(stderr, "\n");
	static bool registered = false;
	if (registered == false) {
		registered = true;
		atexit(_yIO__test_failed_atexit);
	}
}

const char *_yIO_char_to_printstr_in(char dest[5], unsigned char c) {
	if (isprint(c)) {
		dest[0] = c;
	} else {
		dest[0] = '\\';
		dest[1] = 'x';
		const char xdigit[] = "0123456789abcdef";
		dest[2] = xdigit[(c >> 4) & 0xf];
		dest[3] = xdigit[c & 0xf];
	}
	return dest;
}
