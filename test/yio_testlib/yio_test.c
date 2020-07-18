/**
 * @file yio_test.c
 * @date 2020-05-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "yio_test.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline
const char * _yIO__test_get_relative_filepath(const char *file) {
	const char *this_file = __FILE__;
	while (*this_file == *file && *this_file != '\0' && *file != '\0') {
		this_file++;
		file++;
	}
	if (memcmp(file, "test/", 5) == 0) {
		file += 5;
	}
	return file;
}

static
void _yIO__test_failed_atexit(void) {
	_Exit(EXIT_FAILURE);
}

void _yIO__testing(bool verbose, const char *expr, const char *file, int line) {
	if (!verbose) return;
	const char *relative_file = _yIO__test_get_relative_filepath(file);
	printf("%s:%d: Testing %s\n", relative_file, line, expr);
	return;
}

void _yIO__test_failed(const char *expr, const char *file, int line, const char *fmt, ...) {

	fflush(stdout);

	const char *relative_file = _yIO__test_get_relative_filepath(file);
	fprintf(stderr, "%s:%d: ERROR: %s", relative_file, line, expr);
	if (strlen(fmt) != 0 && !(strlen(fmt) == 1 && fmt[0] == ' ')) {
		fprintf(stderr, ": ");
		va_list va;
		va_start(va, fmt);
		vfprintf(stderr, fmt, va);
		va_end(va);
		if (fmt[strlen(fmt)] != '\n') {
			fprintf(stderr, "\n");
		}
	} else {
		fprintf(stderr, "\n");
	}
	fflush(stderr);

	static bool failurer_registered = false;
	if (failurer_registered == false) {
		failurer_registered = true;
		atexit(_yIO__test_failed_atexit);
	}
}
