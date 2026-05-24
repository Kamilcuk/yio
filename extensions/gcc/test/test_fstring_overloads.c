#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Compiler and architecture detection */
#if defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(__arm__) || defined(__thumb__)
#  define MY_ARCH_SUPPORTED 1
#endif

#ifdef MY_ARCH_SUPPORTED
#if defined(__clang__)
#  if defined(__x86_64__) || defined(__i386__) || defined(__aarch64__)
#    define MY_HAS_FLOAT16 1
#  endif
#elif defined(__GNUC__)
#  if defined(__FLT16_MANT_DIG__)
#    define MY_HAS_FLOAT16 1
#  endif
#endif
#if defined(__GNUC__) && !defined(__clang__)
#  if defined(__x86_64__) || defined(__i386__) || defined(__ia64__) || defined(__loongarch__)
#    define MY_HAS_FLOAT128_TYPE 1
#  endif
#  if defined(__x86_64__) || defined(__i386__) || defined(__ia64__)
#    define MY_HAS_FLOAT80_TYPE 1
#  endif
#endif
#if defined(__DEC32_MANT_DIG__)
#  define MY_HAS_DECIMAL32 1
#endif
#if defined(__DEC64_MANT_DIG__)
#  define MY_HAS_DECIMAL64 1
#endif
#if defined(__DEC128_MANT_DIG__)
#  define MY_HAS_DECIMAL128 1
#endif
#ifdef __SIZEOF_INT128__
#  define MY_HAS_INT128 1
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L && !defined(__STDC_NO_COMPLEX__)
#  define MY_HAS_COMPLEX 1
#  include <complex.h>
#endif
#endif

typedef va_list ftest_ctx;
typedef int (*ftest_handler)(ftest_ctx *ctx);

const void *__builtin_fstring(const char *str, ...);

#define TEST_F_CASE(expected, str, ...) \
    do { \
        printf("TESTING: %s\n", str); \
        test_call_impl(expected, __builtin_fstring(str, ##__VA_ARGS__), ""); \
    } while (0)

static void test_call_impl(const char *expected, const void *p, ...) {
    ftest_handler *handlers = (ftest_handler *)p;
    if (!handlers) {
        printf("FAIL: No handlers for %s\n", expected);
        exit(1);
    }
    va_list va;
    va_start(va, p);
    va_arg(va, const char *);
    for (ftest_handler *i = handlers; *i; ++i) {
        (*i)(&va);
    }
    va_end(va);
    printf("OK: %s\n", expected);
}

static int h_int(ftest_ctx *ctx) { int v = va_arg(*ctx, int); assert(v == 123); return 0; }
static int h_uint(ftest_ctx *ctx) { unsigned int v = va_arg(*ctx, unsigned int); assert(v == 123); return 0; }
static int h_long(ftest_ctx *ctx) { long v = va_arg(*ctx, long); assert(v == 123); return 0; }
static int h_ulong(ftest_ctx *ctx) { unsigned long v = va_arg(*ctx, unsigned long); assert(v == 123); return 0; }
static int h_llong(ftest_ctx *ctx) { long long v = va_arg(*ctx, long long); assert(v == 123); return 0; }
static int h_ullong(ftest_ctx *ctx) { unsigned long long v = va_arg(*ctx, unsigned long long); assert(v == 123); return 0; }
static int h_double(ftest_ctx *ctx) { double v = va_arg(*ctx, double); assert(v == 123.0); return 0; }
static int h_ldouble(ftest_ctx *ctx) { long double v = va_arg(*ctx, long double); assert(v == 123.0L); return 0; }
static int h_ptr(ftest_ctx *ctx) { void *v = va_arg(*ctx, void *); assert(v == (void *)123); return 0; }
static int h_bool(ftest_ctx *ctx) { int v = va_arg(*ctx, int); assert(v == 1); return 0; }

#ifdef MY_HAS_INT128
static int h_i128(ftest_ctx *ctx) { __int128 v = va_arg(*ctx, __int128); assert(v == 123); return 0; }
static int h_u128(ftest_ctx *ctx) { unsigned __int128 v = va_arg(*ctx, unsigned __int128); assert(v == 123); return 0; }
#endif

#ifdef MY_HAS_DECIMAL32
static int h_d32(ftest_ctx *ctx) { _Decimal32 v = va_arg(*ctx, _Decimal32); assert(v == 123.0df); return 0; }
#endif
#ifdef MY_HAS_DECIMAL64
static int h_d64(ftest_ctx *ctx) { _Decimal64 v = va_arg(*ctx, _Decimal64); assert(v == 123.0dd); return 0; }
#endif
#ifdef MY_HAS_DECIMAL128
static int h_d128(ftest_ctx *ctx) { _Decimal128 v = va_arg(*ctx, _Decimal128); assert(v == 123.0dl); return 0; }
#endif

#ifdef MY_HAS_COMPLEX
static int h_fcomplex(ftest_ctx *ctx) { float complex v = va_arg(*ctx, float complex); assert(crealf(v) == 123.0f && cimagf(v) == 0.0f); return 0; }
static int h_dcomplex(ftest_ctx *ctx) { double complex v = va_arg(*ctx, double complex); assert(creal(v) == 123.0 && cimag(v) == 0.0); return 0; }
static int h_lcomplex(ftest_ctx *ctx) { long double complex v = va_arg(*ctx, long double complex); assert(creall(v) == 123.0L && cimagl(v) == 0.0L); return 0; }
#endif

#ifdef MY_HAS_INT128
#define H_I128 , (__int128 *)0, h_i128, (unsigned __int128 *)0, h_u128
#else
#define H_I128
#endif

#ifdef MY_HAS_DECIMAL32
#define H_D32 , (_Decimal32 *)0, h_d32
#else
#define H_D32
#endif

#ifdef MY_HAS_DECIMAL64
#define H_D64 , (_Decimal64 *)0, h_d64
#else
#define H_D64
#endif

#ifdef MY_HAS_DECIMAL128
#define H_D128 , (_Decimal128 *)0, h_d128
#else
#define H_D128
#endif

#ifdef MY_HAS_COMPLEX
#define H_COMPLEX , (float complex *)0, h_fcomplex, (double complex *)0, h_dcomplex, (long double complex *)0, h_lcomplex
#else
#define H_COMPLEX
#endif

#define ALL_HANDLERS \
    (int *)0, h_int, \
    (unsigned int *)0, h_uint, \
    (long *)0, h_long, \
    (unsigned long *)0, h_ulong, \
    (long long *)0, h_llong, \
    (unsigned long long *)0, h_ullong, \
    (double *)0, h_double, \
    (long double *)0, h_ldouble, \
    (void **)0, h_ptr, \
    (_Bool *)0, h_bool, \
    (char *)0, h_int, \
    (signed char *)0, h_int, \
    (short *)0, h_int \
    H_I128 H_D32 H_D64 H_D128 H_COMPLEX

int main() {
#ifndef MY_ARCH_SUPPORTED
    return 0;
#else
    char c = 123;
    signed char sc = 123;
    short s = 123;
    int i = 123;
    long l = 123;
    long long ll = 123;
    bool b = true;
    void *ptr = (void *)123;
    double d = 123.0;
    long double ld = 123.0;

    TEST_F_CASE("123", "{c}", ALL_HANDLERS);
    TEST_F_CASE("123", "{sc}", ALL_HANDLERS);
    TEST_F_CASE("123", "{s}", ALL_HANDLERS);
    TEST_F_CASE("123", "{i}", ALL_HANDLERS);
    TEST_F_CASE("123", "{l}", ALL_HANDLERS);
    TEST_F_CASE("123", "{ll}", ALL_HANDLERS);
    TEST_F_CASE("true", "{b}", ALL_HANDLERS);
    TEST_F_CASE("123", "{ptr}", ALL_HANDLERS);
    TEST_F_CASE("123.0", "{d}", ALL_HANDLERS);
    TEST_F_CASE("123.0L", "{ld}", ALL_HANDLERS);

#ifdef MY_HAS_INT128
    __int128 i128 = 123;
    TEST_F_CASE("123", "{i128}", ALL_HANDLERS);
#endif

#ifdef MY_HAS_DECIMAL32
    _Decimal32 d32 = 123.0df;
    TEST_F_CASE("123.0", "{d32}", ALL_HANDLERS);
#endif

#ifdef MY_HAS_COMPLEX
    double complex dc = 123.0 + 0.0 * I;
    TEST_F_CASE("123+0i", "{dc}", ALL_HANDLERS);
#endif

    return 0;
#endif
}
