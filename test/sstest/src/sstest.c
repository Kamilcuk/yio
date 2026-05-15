#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "sstest.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
#include <dlfcn.h>
#include <sys/wait.h>
#endif

#ifndef SSTEST_USE_COLORS
#define SSTEST_USE_COLORS  1
#endif

static const char *const GREEN  = SSTEST_USE_COLORS ? "\33[32m" : "";
static const char *const RED    = SSTEST_USE_COLORS ? "\33[91m" : "";
static const char *const YELLOW = SSTEST_USE_COLORS ? "\33[93m" : "";
static const char *const CYAN   = SSTEST_USE_COLORS ? "\33[36m" : "";
static const char *const RESET  = SSTEST_USE_COLORS ? "\33[0m" : "";

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

#if SSTEST_USE_BACKTRACE && defined(__linux__)

static inline void print_source_context(const char *filename, int line_num) {
	FILE *f = fopen(filename, "r");
	if (f == NULL) return;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int current_line = 0;
	while ((read = getline(&line, &len, f)) != -1) {
		current_line++;
		if (current_line >= line_num - 1 && current_line <= line_num + 1) {
			const bool is_target = (current_line == line_num);
			const char *color = is_target ? RED : "";
			const char *reset = is_target ? RESET : "";
			fprintf(stderr, "%s      %s %4d | %s%s", color, is_target ? "->" : "  ", current_line, line, reset);
		}
		if (current_line > line_num + 1) break;
	}
	free(line);
	fclose(f);
}

static inline void run_addr2line(const char *exe, uintptr_t address) {
	int pipefd[2];
	if (pipe(pipefd) == -1) return;

	pid_t pid = fork();
	if (pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}
	if (pid == 0) {
		close(pipefd[0]);
		if (pipefd[1] != STDOUT_FILENO) {
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		char addr_str[32];
		snprintf(addr_str, sizeof(addr_str), "0x%lx", address);
		execlp("addr2line", "addr2line", "-Cfi", "-e", exe, addr_str, (char *)NULL);
		_exit(1);
	}
	close(pipefd[1]);

	FILE *f = fdopen(pipefd[0], "r");
	if (f) {
		char fbuf[1024];
		char lbuf[1024];
		while (fgets(fbuf, sizeof(fbuf), f) && fgets(lbuf, sizeof(lbuf), f)) {
			fbuf[strcspn(fbuf, "\n")] = '\0';
			lbuf[strcspn(lbuf, "\n")] = '\0';
			if (strcmp(lbuf, "??:0") == 0 || strcmp(lbuf, "??:?") == 0) continue;

			char *colon = strrchr(lbuf, ':');
			if (colon) {
				*colon = '\0';
				int line_num = atoi(colon + 1);
				fprintf(stderr, "    File %s\"%s\"%s, line %s%s%s, in %s%s()%s\n",
					GREEN, hfile(lbuf), RESET, GREEN, colon + 1, RESET, GREEN, fbuf, RESET);
				print_source_context(lbuf, line_num);
			}
		}
		fclose(f);
	} else {
		close(pipefd[0]);
	}
	waitpid(pid, NULL, 0);
}

static inline char **print_stacktrace(void) {
	void *array[50];
	const size_t size = backtrace(array, sizeof(array) / sizeof(*array));
	char **ss = backtrace_symbols(array, size);
	if (ss == NULL) return NULL;

	for (size_t i = 0; i < size; ++i) {
		Dl_info info;
		void *addr = array[i];
		if (i > 0) addr = (void *)((uintptr_t)addr - 1);

		if (dladdr(addr, &info) && info.dli_fname) {
			// Skip frames from sstest itself
			if (info.dli_sname && (
				strcmp(info.dli_sname, "print_stacktrace") == 0 ||
				strcmp(info.dli_sname, "sstest_post") == 0 ||
				strcmp(info.dli_sname, "sighandler") == 0)) {
				continue;
			}
			fprintf(stderr, "  %s^-> %s%s\n", CYAN, ss[i], RESET);
			uintptr_t offset = (uintptr_t)addr - (uintptr_t)info.dli_fbase;
			run_addr2line(info.dli_fname, offset);
		} else {
			fprintf(stderr, "  %s^-> %s%s\n", CYAN, ss[i], RESET);
		}
	}
	fflush(stderr);
	return ss;
}

/// The sighandler for printing stacktrace.
static void sighandler(int sig) {
	signal(SIGSEGV, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	fflush(stdout);
	fprintf(stderr, "%sSSTEST: Error: received signal %d:%s%s\n", RED, sig, strsignal(sig), RESET);
	free(print_stacktrace());
}

/// For testing, by deafult, disable buffering and register our printers.
__attribute__((__constructor__)) static void disable_buffering(void) {
	setvbuf(stdout, 0, _IOLBF, 0);
	setvbuf(stderr, 0, _IOLBF, 0);
	signal(SIGSEGV, sighandler);
	signal(SIGABRT, sighandler);
}


#else // SSTEST_USE_BACKTRACE && __linux__

static inline void *print_stacktrace(void) { return NULL; }

#endif // SSTEST_USE_BACKTRACE && __linux__

/// The atexit callback.
static void sstest_atexit(void) {
	printf("SSTEST: %sFailing!%s Number of failures: %u\n", RED, RESET, failures);
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
			PUT(GREEN, "OK: ", hnull(p->expr), RESET, "\n");
			fflush(stderr);
		}
	} else {
		print_prefix(p);
		if (p->warn) {
			PUT(YELLOW, "WARN");
		} else if (p->fail) {
			PUT(RED, "FATAL ERROR");
		} else {
			PUT(RED, "ERROR");
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
			PUT(RESET, "\n");
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
