#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "sstest.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if __linux__ && __GLIBC__ && __GNUC__
#define SSTEST_USE_BACKTRACE  1
#endif
#ifndef SSTEST_USE_BACKTRACE
#define SSTEST_USE_BACKTRACE  0
#endif
#if SSTEST_USE_BACKTRACE
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#endif

#ifndef SSTEST_USE_COLORS
#define SSTEST_USE_COLORS  1
#endif

#if SSTEST_USE_COLORS
#define GREEN   "\33[32m"
#define RED     "\33[91m"
#define YELLOW  "\33[93m"
#define RESET   "\33[0m"
#else
#define GREEN   ""
#define RED     ""
#define YELLOW  ""
#define RESET   ""
#endif

#define PUT_1(x)       fputs(x, stderr)
#define PUT_2(x, ...)  PUT_1(x); PUT_1(__VA_ARGS__)
#define PUT_3(x, ...)  PUT_1(x); PUT_2(__VA_ARGS__)
#define PUT_4(x, ...)  PUT_1(x); PUT_3(__VA_ARGS__)
#define PUT_5(x, ...)  PUT_1(x); PUT_4(__VA_ARGS__)
#define PUT_6(x, ...)  PUT_1(x); PUT_5(__VA_ARGS__)
#define PUT_7(x, ...)  PUT_1(x); PUT_6(__VA_ARGS__)
#define PUT_8(x, ...)  PUT_1(x); PUT_7(__VA_ARGS__)
#define PUT_9(x, ...)  PUT_1(x); PUT_8(__VA_ARGS__)
#define PUT_N(_9,_8,_7,_6,_5,_4,_3,_2,_1,N,...) PUT_##N
/// Small wrapper so thatI do not have to use fprintf or type fputs so much.
#define PUT(...)  do{ PUT_N(__VA_ARGS__,9,8,7,6,5,4,3,2,1)(__VA_ARGS__); } while(0)

/// Keep track of how many failures we had.
static unsigned int failures = 0;

#if SSTEST_USE_BACKTRACE

static inline char **print_stacktrace(void) {
	void *array[50];
	const size_t size = backtrace(array, sizeof(array) / sizeof(*array));
	char **ss = backtrace_symbols(array, size);
	if (ss == NULL) return NULL;
	for (size_t i = 0; i < size; ++i) {
		const char *pos = ss[i];
		PUT(pos, "\n");
		//
		// Parse. hopefully as rubust as possible.
		const char *const exe = pos;
		pos = strchr(pos, '(');
		if (pos == NULL) continue;
		const int exelen = pos - exe;
		pos++;
		const bool hasfunc = pos[0] != '+';
		const char *const func = hasfunc ? pos : NULL;
		if (hasfunc) {
			pos = strchr(pos, '+');
			if (pos == NULL) continue;
		}
		const int funclen = hasfunc ? pos - func : 0;
		const char *const addr = pos;
		if (addr[0] != '+') continue;
		if (addr[1] != '0') continue;
		if (addr[2] != 'x') continue;
		pos = strchr(pos, ')');
		if (pos == NULL) continue;
		const int addrlen = pos - addr;
		if (strspn(&addr[3], "0123456789abcdefABCDEF") + 3 != (size_t)addrlen) continue;
		// We can't have no ampersand, cause shell quoting.
		if (memchr(exe, '\'', pos - exe) != NULL) continue;
		//
		// Run shell to print backtrace
		char cmd[4096];
		int len = 0;
		if (funclen != 0 && func != NULL) {
			len = snprintf(cmd, sizeof(cmd),
					"set -- '%.*s' '%.*s' '%.*s';"
					"addr2line -Cfipe >&2 "
					"\"$(debuginfod-find debuginfo \"$1\" 2>/dev/null||echo \"$1\")\" "
					"$(($2+0x$(nm -D \"$1\"|awk -v r=\" $3\" '$0~r{print $1;exit}')))"
					,
					exelen, exe,
					addrlen, addr,
					funclen, func);
		} else {
			len = snprintf(cmd, sizeof(cmd),
					"set -- '%.*s';"
					"addr2line -Cfipe >&2 "
					"\"$(debuginfod-find debuginfo \"$1\" 2>/dev/null||echo \"$1\")\" "
					"'%.*s'"
					,
					exelen, exe,
					addrlen, addr);
		}
		if ((size_t)len >= sizeof(cmd) - 1) continue;
		PUT("  ^-> ");
		fflush(stderr);
		const int res = system(cmd);
		(void)res;
	}
	fflush(stderr);
	return ss;
}

/// The sighandler for printing stacktrace.
static void sighandler(int sig) {
	signal(SIGSEGV, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	fflush(stdout);
	fprintf(stderr, RED"SSTEST: Error: received signal %d:%s"RESET"\n", sig, strsignal(sig));
	print_stacktrace();
}

/// For testing, by deafult, disable buffering and register our printers.
__attribute__((__constructor__)) static void disable_buffering(void) {
	setvbuf(stdout, 0, _IOLBF, 0);
	setvbuf(stderr, 0, _IOLBF, 0);
	signal(SIGSEGV, sighandler);
	signal(SIGABRT, sighandler);
}


#else // SSTEST_USE_BACKTRACE

static inline void *print_stacktrace(void) {}

#endif // SSTEST_USE_BACKTRACE

/// The atexit callback.
static void sstest_atexit(void) {
	printf("SSTEST: "RED"Failing!"RESET" Number of failures: %u\n", failures);
	_Exit(EXIT_FAILURE);
}

/// Increment failures, and register on exit handler.
static inline void sstest_inc_failures(void) {
	failures++;
	static bool failurer_registered = false;
	if (failurer_registered == false) {
		failurer_registered = true;
		atexit(sstest_atexit);
	}
}

/// Determine if we are verbose.
static inline bool isverbose(void) {
	const char *env = getenv("SSTEST");
	return env != NULL && strlen(env) > 0;
}

/// Handle file. Remove common prefix from filenames.
static const char *hfile(const char *str) {
#ifdef SSTEST_ROOT
	const char *const root = SSTEST_XSTRING(SSTEST_ROOT);
	const size_t rootlen = strlen(root);
	if (strncmp(str, root, rootlen) == 0) {
		str = str + rootlen;
		while (*str == '/') ++str;
	}
#endif
	return str;
}

/// Handle null, safety.
static const char *hnull(const char *s) {
	return s ? s : "(null)";
}

/// Print line prefix.
static void print_prefix(const struct sstest_pos *p) {
	fflush(stdout);
	PUT(hfile(p->file), ":", hnull(p->line), ":", hnull(p->func), ":", " ");
}

void sstest_pre(const struct sstest_pos *p) {
	if (p->verbose || isverbose()) {
		print_prefix(p);
		PUT("Testing ", hnull(p->expr), "\n");
		fflush(stderr);
	}
}

bool sstest_post(const struct sstest_pos *p, bool result, const char *fmt, ...) {
	if (result) {
		if (!p->quiet) {
			print_prefix(p);
			PUT(GREEN "OK: ", hnull(p->expr), RESET "\n");
			fflush(stderr);
		}
	} else {
		print_prefix(p);
		if (p->warn) {
			PUT(YELLOW "WARN");
		} else if (p->fail) {
			PUT(RED "FATAL ERROR");
		} else {
			PUT(RED "ERROR");
		}
		PUT(": ", hnull(p->expr));
		if (strlen(fmt) != 0 && (strlen(fmt) > 1 || fmt[0] != '\377')) {
			PUT(": ");
			va_list va;
			va_start(va, fmt);
			vfprintf(stderr, fmt, va);
			va_end(va);
			PUT(RESET);
			if (fmt[strlen(fmt)] != '\n') {
				PUT("\n");
			}
		} else {
			PUT(RESET "\n");
		}
		if (!p->warn) {
			sstest_inc_failures();
		}
		if (p->fail) {
			free(print_stacktrace());
			exit(EXIT_FAILURE);
		}
		fflush(stderr);
	}
	return result;
}

bool sstest_is_in_valgrind(void) {
#if __linux__
	const char *p = getenv("LD_PRELOAD");
	return p != NULL && (strstr(p, "/valgrind/") != NULL || strstr(p, "/vgpreload") != NULL);
#else
	return false;
#endif
}
