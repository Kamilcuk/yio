#ifndef EXTENSIONS_GCC_CONFTEST_H_
#define EXTENSIONS_GCC_CONFTEST_H_

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef va_list ftest_ctx;
typedef int (*ftest_handler)(ftest_ctx *ctx);

#if defined(__has_attribute)
#if __has_attribute(fstring_format)
#define FSTRING_FORMAT(N) __attribute__((fstring_format(N)))
#endif
#endif
#ifndef FSTRING_FORMAT
#define FSTRING_FORMAT(N)
#endif

const void *__builtin_fstring(const char *str, ...);
#define test_f(str, ...) (ftest_handler *)__builtin_fstring(str, ##__VA_ARGS__), ""

FSTRING_FORMAT(2)
static void ftest_call(ftest_handler handlers[], const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  printf("fmt: %s\n", fmt);
  for (ftest_handler *i = handlers; *i; ++i) {
    (*i)(&va);
  }
}

#define FTEST_ASSERT(expr) \
  do { \
    if (!(expr)) { \
      fprintf(stderr, "Assertion failed: %s at %s:%d\n", #expr, __FILE__, __LINE__); \
      exit(1); \
    } \
  } while (0)

#define FTEST_CHECK_EQ(expected, ...) \
  do { \
    char buf[1024]; \
    int err = yio_snprint(buf, sizeof(buf), __VA_ARGS__); \
    assert(err > 0); \
    if (strcmp(expected, buf) != 0) { \
      fprintf( \
          stderr, \
          "Check failed!\nExpected: %s\nActual:   %s\nAt %s:%d\n", \
          expected, \
          buf, \
          __FILE__, \
          __LINE__ \
      ); \
      exit(1); \
    } \
  } while (0)

#endif
