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
  if (!handlers) return;
  va_list va;
  va_start(va, fmt);
  printf("fmt: %s\n", fmt);
  for (ftest_handler *i = handlers; *i; ++i) {
    (*i)(&va);
  }
  va_end(va);
}

static int ftest_h_int(ftest_ctx *ctx) {
  assert(ctx);
  int val = va_arg(*ctx, int);
  printf("h_int: %d\n", val);
  return 0;
}

static int ftest_h_double(ftest_ctx *ctx) {
  assert(ctx);
  double val = va_arg(*ctx, double);
  printf("h_double: %f\n", val);
  return 0;
}

static int ftest_h_float(ftest_ctx *ctx) {
  assert(ctx);
  // float is promoted to double in variadic arguments
  double val = va_arg(*ctx, double);
  printf("h_float: %f\n", val);
  return 0;
}

static int ftest_h_long(ftest_ctx *ctx) {
  assert(ctx);
  long val = va_arg(*ctx, long);
  printf("h_long: %ld\n", val);
  return 0;
}

static int ftest_h_llong(ftest_ctx *ctx) {
  assert(ctx);
  long long val = va_arg(*ctx, long long);
  printf("h_llong: %lld\n", val);
  return 0;
}

static int ftest_h_ptr(ftest_ctx *ctx) {
  assert(ctx);
  void *val = va_arg(*ctx, void *);
  printf("h_ptr: %p\n", val);
  return 0;
}

#define FTEST_HANDLERS \
  (int *)0, ftest_h_int, \
  (long *)0, ftest_h_long, \
  (long long *)0, ftest_h_llong, \
  (double *)0, ftest_h_double, \
  (float *)0, ftest_h_float, \
  (void **)0, ftest_h_ptr

#define FTEST_F(str, ...) test_f(str, FTEST_HANDLERS, ##__VA_ARGS__)
#define FTEST_PRINT_F(str, ...) ftest_call(FTEST_F(str, ##__VA_ARGS__), ##__VA_ARGS__)

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
