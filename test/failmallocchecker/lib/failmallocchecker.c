/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "failmallocchecker.h"
#if !FMC_DISABLE
#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

struct fmc_ctx_s {
#if FMC_USE_METHOD == FMC_METHOD_GLIBC_MALLOC_HOOK
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

#if FMC_USE_METHOD == FMC_METHOD_GLIBC_MALLOC_HOOK
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
#elif FMC_USE_METHOD == FMC_METHOD_WRAP_MALLOC
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
#elif FMC_USE_METHOD == FMC_METHOD_CALL___LIBC
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
#else
#error
#endif


#ifdef __GNUC__
__attribute__((__format__(__printf__, 1, 2)))
#endif
static void _fmc_printf(const char *fmt, ...) {
	int ret = 0;
	FMC_IGNORE_BLOCK() {
		ret = fflush(stdout);
		assert(ret == 0); (void)ret;
		ret = fflush(stderr);
		assert(ret == 0); (void)ret;
		va_list va;
		va_start(va, fmt);
		ret = vfprintf(stderr, fmt, va);
		va_end(va);
		assert(ret > 0); (void)ret;
	}
}

void _fmc_ignore(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	t->ignore = true;
}

void _fmc_noignore(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	t->ignore = false;
}

void _fmc_init(const char *file, int line, const char *func) {
	struct fmc_ctx_s *t = &fmc_ctx;
	const struct fmc_ctx_s reset = { 0 };
	*t = reset;
	fmc_hook_init();

	// poke standard stream so that they allocate memory
	setvbuf(stdout, NULL, _IOLBF, 256);
	setvbuf(stderr, NULL, _IOLBF, 256);

	_fmc_printf("fmc: init: %s:%d:%s\n",
			strrchr(file, '/') ? strrchr(file, '/') + 1 : "", line, func);
}

int _fmc_continue(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	bool ret = !(t->counted && t->failing == t->count);
	if (ret == false) {
		fmc_unhook();
		_fmc_printf("fmc: end\n");
	}
	return ret;
}

void _fmc_next(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	_fmc_print();
	t->pos = 0;
	if (!t->counted) {
		t->counted = true;
	} else {
		t->failing++;
	}
}

void _fmc_print(void) {
	struct fmc_ctx_s *t = &fmc_ctx;
	_fmc_printf("fmc: counted=%d count=%d failing=%d pos=%d\n", t->counted, t->count, t->failing, t->pos);
}

#endif
