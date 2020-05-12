/**
 * @file yio_test.c
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_test.h"
#include <stdlib.h>
#include <stdbool.h>

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
