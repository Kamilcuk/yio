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

static unsigned failures = 0;

static inline
const char * YYIO__test_get_relative_filepath(const char *file) {
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
void YYIO__test_failed_atexit(void) {
	printf("YYIO_TEST: failures: %u\n", failures);
	_Exit(EXIT_FAILURE);
}

int YYIO__testing(bool verbose, const char *expr, const char *file, int line) {
	fflush(stdout);
	fflush(stderr);
	if (!verbose) return 0;
	const char *relative_file = YYIO__test_get_relative_filepath(file);
	printf("%s:%d: Testing %s\n", relative_file, line, expr);
	fflush(stdout);
	return 0;
}

bool YYIO__test_failed(int unused, bool result,
		int flags, const char *expr, const char *file, int line, const char *fmt, ...) {
	(void)unused;
	if (result) return false;

	const bool fail = !(flags & YYIO_TEST_FLAG_NOFAIL);
	const bool assert = flags & YYIO_TEST_FLAG_ASSERT;

	fflush(stdout);

	const char *relative_file = YYIO__test_get_relative_filepath(file);
	fprintf(stderr, "%s:%d: %s: %s", relative_file, line, fail ? "ERROR" : "WARNING", expr);
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

	if (assert) {
		abort();
	}

	if (fail) {
		failures++;
		static bool failurer_registered = false;
		if (failurer_registered == false) {
			failurer_registered = true;
			atexit(YYIO__test_failed_atexit);
		}
	}

	return true;
}

bool YYIO_test_is_in_valgrind(void) {
	// long double not supported in valgrind
#if __linux__
	const char *p = getenv("LD_PRELOAD");
	if (p == NULL) return 0;
	return strstr(p, "/valgrind/") != NULL || strstr(p, "/vgpreload") != NULL;
#else
	return false;
#endif
}

#if __linux__ && __GLIBC__ && __GNUC__
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

// https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-program-crashes
static void sighandler(int sig) {
	signal(SIGSEGV, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	//
	void *array[50];
	const size_t size = backtrace(array, sizeof(array)/sizeof(*array));
	fprintf(stderr, "Error: signal %d:%s\n", sig, strsignal(sig));
	char **ss = backtrace_symbols(array, size);
	for (size_t i = 0; i < size; ++i) {
		char exe[2048], addr[20];
		if (sscanf(ss[i], "%2047[^(](+%19[^)]", exe, addr) == 2) {
			char cmd[4096];
			snprintf(cmd, sizeof(cmd), "addr2line -Cfip -e %s %s", exe, addr);
			const int r = system(cmd);
			(void)r;
		} else {
			printf("%s\n", ss[i]);
		}
	}
}

__attribute__((__constructor__))
static void disable_buffering(void) {
	setvbuf(stdout, 0, _IOLBF, 0);
	setvbuf(stderr, 0, _IOLBF, 0);
	signal(SIGSEGV, sighandler);
	signal(SIGABRT, sighandler);
}

bool YYIO_tty(void) {
	return isatty(STDOUT_FILENO);
}
#else
bool YYIO_tty(void) {
	return 0;
}
#endif
