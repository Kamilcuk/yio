/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "failmallocchecker.h"
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

static inline
void writestr(const char *str) {
	int ret = fflush(stdout);
	assert(ret == 0); (void)ret;
	ret = fflush(stderr);
	assert(ret == 0); (void)ret;
	ssize_t sret = write(STDOUT_FILENO, str, strlen(str));
	assert(sret > 0); (void)sret;
}

// #define FMC_USE_GLIBC_MALLOC_HOOK
#define FMC_USE_WRAP_MALLOC

struct fmc_ctx_s {
#ifdef FMC_USE_GLIBC_MALLOC_HOOK
	struct {
		void *(*malloc)(size_t size, const void *caller);
		void *(*realloc)(void *ptr, size_t size, const void *caller);
		void *(*memalign)(size_t alignment, size_t size, const void *caller);
	} orig;
#endif
	// the count of *alloc calls in the code
	unsigned count;
	// the current position of *alloc call in the code
	unsigned pos;
	// This number *alloc call will fail.
	unsigned failing;
	// If counting alloc calls.
	bool counted;
	// Ignore - just forward.
	bool ignore;
};

static struct fmc_ctx_s fmc_ctx;

static bool fmc_fail_it(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	if (t->ignore) {
		return false;
	}
	if (!t->counted) {
		assert(t->count < UINT_MAX);
		t->count++;
	} else {
		if (t->pos++ == t->failing) {
			return true;
		}
	}
	return false;
}

#ifdef FMC_USE_GLIBC_MALLOC_HOOK
#include <malloc.h>
static void fmc_unhook(void);
static void fmc_rehook(void);
static void *fmc_malloc_hook(size_t size, const void *caller) {
	(void)caller;
	if (fmc_fail_it()) return NULL;
	fmc_unhook();
	void *p = malloc(size);
	fmc_rehook();
	return p;
}
static void *fmc_realloc_hook(void *ptr, size_t size, const void *caller) {
	(void)caller;
	if (fmc_fail_it()) return NULL;
	fmc_unhook();
	void *p = realloc(ptr, size);
	fmc_rehook();
	return p;
}

static void *fmc_memalign_hook(size_t alignment, size_t size, const void *caller) {
	(void)caller;
	if (fmc_fail_it()) return NULL;
	fmc_unhook();
	void *p = memalign(alignment, size);
	fmc_rehook();
	return p;
}
static void fmc_unhook(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	__malloc_hook = t->orig.malloc;
	__realloc_hook = t->orig.realloc;
	__memalign_hook = t->orig.memalign;
}
static void fmc_rehook(void) {
	__malloc_hook = fmc_malloc_hook;
	__realloc_hook = fmc_realloc_hook;
	__memalign_hook = fmc_memalign_hook;
}
static void fmc_hook_init(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	t->orig.malloc = __malloc_hook;
	t->orig.realloc = __realloc_hook;
	t->orig.memalign = __memalign_hook;
	fmc_rehook();
}
#elif defined FMC_USE_WRAP_MALLOC
extern void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size) {
	return fmc_fail_it() ? NULL : __real_malloc(size);
}
extern void *__real_calloc(size_t nmemb, size_t size);
void *__wrap_calloc(size_t nmemb, size_t size) {
	return fmc_fail_it() ? NULL : __real_calloc(nmemb, size);
}
extern void *__real_realloc(void *ptr, size_t size);
void *__wrap_realloc(void *ptr, size_t size) {
	return fmc_fail_it() ? NULL : __real_realloc(ptr, size);
}
static void fmc_hook_init(void) {}
static void fmc_unhook(void) {}
#else
extern void *__libc_malloc(size_t size);
void *malloc(size_t s) {
	return fmc_fail_it() ? NULL : __libc_malloc(s);
}
extern void *__libc_calloc(size_t nmemb, size_t size);
void *calloc(size_t nmemb, size_t size) {
	return fmc_fail_it() ? NULL : __libc_calloc(nmemb, size);
}
extern void *__libc_realloc(void *ptr, size_t size);
void *realloc(void *ptr, size_t size) {
	return fmc_fail_it() ? NULL : __libc_realloc(ptr, size);
}
static void fmc_hook_init(void) {}
static void fmc_unhook(void) {}
#endif


#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
static void fmc_printf(const char *fmt, ...) {
	int ret = fflush(stdout);
	assert(ret == 0); (void)ret;
	FMC_IGNORE() {
		va_list va;
		va_start(va, fmt);
		ret = vprintf(fmt, va);
		va_end(va);
	}
	assert(ret > 0); (void)ret;
}

void fmc_ignore(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	t->ignore = true;
}

void fmc_noignore(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	t->ignore = false;
}

void fmc_init(const char *file, int line, const char *func) {
	struct fmc_ctx_s *t = &fmc_ctx;
	const struct fmc_ctx_s reset = { 0 };
	*t = reset;
	fmc_hook_init();
	fmc_printf("fmc: init: %s:%d:%s\n",
			strrchr(file, '/') ? strrchr(file, '/') + 1 : "", line, func);
}

int fmc_continue(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	bool ret = !(t->counted && t->failing == t->count);
	if (ret == false) {
		fmc_unhook();
		fmc_printf("fmc: end\n");
	}
	return ret;
}

void fmc_next(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	fmc_print();
	t->pos = 0;
	if (!t->counted) {
		t->counted = true;
	} else {
		t->failing++;
	}
}

void fmc_print(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	fmc_printf("fmc: counted=%d count=%d failing=%d pos=%d\n", t->counted, t->count, t->failing, t->pos);
}




