#include <yio.h>
#include <yio_test.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>
#include <time.h>
#include <sys/time.h>

#if __has_include(<uchar.h>)
#  include <uchar.h>
#  define MY_HAS_UCHAR_H 1
#endif

/* Compiler and architecture detection for extended type support */

#if defined(__x86_64__) || defined(__i386__)
#  define MY_IS_X86 1
#endif
#if defined(__aarch64__)
#  define MY_IS_AARCH64 1
#endif
#if defined(__arm__) || defined(__thumb__)
#  define MY_IS_ARM 1
#endif

#if defined(MY_IS_X86) || defined(MY_IS_AARCH64) || defined(MY_IS_ARM)
#  define MY_ARCH_SUPPORTED 1
#endif

#ifdef MY_ARCH_SUPPORTED

/* _Float16 availability:
 * Clang supports it on x86 and aarch64.
 * GCC provides __FLT16_MANT_DIG__ when supported.
 */
#if defined(__clang__)
#  if defined(MY_IS_X86) || defined(MY_IS_AARCH64)
#    define MY_HAS_FLOAT16 1
#  endif
#elif defined(__GNUC__)
#  if defined(__FLT16_MANT_DIG__)
#    define MY_HAS_FLOAT16 1
#  endif
#endif

/* GCC-specific extended floating point types.
 * __float128 and __float80 are available on specific architectures as aliases
 * to _Float128 and _Float64x.
 */
#if defined(__GNUC__) && !defined(__clang__)
#  if defined(MY_IS_X86) || defined(__ia64__) || defined(__loongarch__) || \
      (defined(__hppa__) && defined(__hpux__)) || \
      (defined(__powerpc__) && defined(__linux__) && defined(__VSX__))
#    define MY_HAS_FLOAT128_TYPE 1
#  endif
#  if defined(MY_IS_X86) || defined(__ia64__)
#    define MY_HAS_FLOAT80_TYPE 1
#  endif
#endif

/* Standard C extended floating point types (_FloatN and _FloatNx).
 * Detected via predefined mantissa macros.
 */
#if defined(__GNUC__)
#  if defined(__FLT32_MANT_DIG__)
#    define MY_HAS_FLOAT32 1
#  endif
#  if defined(__FLT64_MANT_DIG__)
#    define MY_HAS_FLOAT64 1
#  endif
#  if defined(__FLT128_MANT_DIG__)
#    define MY_HAS_FLOAT128 1
#  endif
#  if defined(__FLT32X_MANT_DIG__)
#    define MY_HAS_FLOAT32X 1
#  endif
#  if defined(__FLT64X_MANT_DIG__)
#    define MY_HAS_FLOAT64X 1
#  endif
#  if defined(__FLT128X_MANT_DIG__)
#    define MY_HAS_FLOAT128X 1
#  endif
#endif

/* Decimal floating point support (_DecimalN).
 * Detected via predefined mantissa macros.
 */
#if defined(__DEC32_MANT_DIG__)
#  define MY_HAS_DECIMAL32 1
#endif
#if defined(__DEC64_MANT_DIG__)
#  define MY_HAS_DECIMAL64 1
#endif
#if defined(__DEC128_MANT_DIG__)
#  define MY_HAS_DECIMAL128 1
#endif

/* ISO/IEC TR 18037 fixed-point support. */
#ifdef __STDC_FIXED_POINT__
#  define MY_HAS_STDFIX 1
#endif

/* 128-bit integer support. */
#ifdef __SIZEOF_INT128__
#  define MY_HAS_INT128 1
#endif

/* Arbitrary-width integer support (_BitInt). */
#ifdef __BITINT_MAXWIDTH__
#  define MY_HAS_BITINT 1
#endif

/* Clang-specific arbitrary-width integer support (_ExtInt). */
#if defined(__clang__)
#  define MY_HAS_EXTINT 1
#endif

/* Complex and Imaginary support */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L && !defined(__STDC_NO_COMPLEX__)
#  define MY_HAS_COMPLEX 1
#  include <complex.h>
/* _Imaginary is optional and often not supported even if complex is. */
#  if defined(_Imaginary_I)
#    define MY_HAS_IMAGINARY 1
#  endif
#endif

#endif /* MY_ARCH_SUPPORTED */

int main() {
#ifndef MY_ARCH_SUPPORTED
	return 0;
#else
	/* Basic types */
	YIO_TEST_EQ("A", "{}", (char)'A');
	YIO_TEST_EQ("1", "{}", (signed char)1);
	YIO_TEST_EQ("1", "{}", (unsigned char)1);
	YIO_TEST_EQ("1", "{}", (short)1);
	YIO_TEST_EQ("1", "{}", (unsigned short)1);
	YIO_TEST_EQ("1", "{}", (int)1);
	YIO_TEST_EQ("1", "{}", (unsigned int)1);
	YIO_TEST_EQ("1", "{}", (long)1);
	YIO_TEST_EQ("1", "{}", (unsigned long)1);
	YIO_TEST_EQ("1", "{}", (long long)1);
	YIO_TEST_EQ("1", "{}", (unsigned long long)1);
	YIO_TEST_EQ("true", "{}", (bool)true);
	YIO_TEST_EQ("65", "{}", 'A'); // 'A' is int in C
	YIO_TEST_EQ("hello", "{}", "hello");

	/* Floats - default is 'g' format */
	YIO_TEST_EQ("1", "{}", 1.0f);
	YIO_TEST_EQ("1", "{}", 1.0);
	YIO_TEST_EQ("1", "{}", 1.0L);

	YIO_TEST_EQ("1.000000", "{:f}", 1.0f);
	YIO_TEST_EQ("1.000000", "{:f}", 1.0);
	YIO_TEST_EQ("1.000000", "{:f}", 1.0L);

#ifdef MY_HAS_INT128
	YIO_TEST_EQ("1", "{}", (__int128)1);
	YIO_TEST_EQ("1", "{}", (unsigned __int128)1);
#endif

#ifdef MY_HAS_BITINT
#if __BITINT_MAXWIDTH__ >= 1
	YIO_TEST_EQ("1", "{}", (unsigned _BitInt(1))1);
#endif
#if __BITINT_MAXWIDTH__ >= 2
	YIO_TEST_EQ("1", "{}", (_BitInt(2))1);
#endif
#if __BITINT_MAXWIDTH__ >= 37
	YIO_TEST_EQ("1", "{}", (_BitInt(37))1);
#endif
#if __BITINT_MAXWIDTH__ >= 128
	YIO_TEST_EQ("1", "{}", (unsigned _BitInt(128))1);
#endif
#endif

#ifdef MY_HAS_EXTINT
	YIO_TEST_EQ("1", "{}", (unsigned _ExtInt(1))1);
	YIO_TEST_EQ("1", "{}", (_ExtInt(2))1);
	YIO_TEST_EQ("1", "{}", (_ExtInt(37))1);
	YIO_TEST_EQ("1", "{}", (unsigned _ExtInt(128))1);
#endif

#ifdef MY_HAS_STDFIX
	YIO_TEST_EQ("0.5", "{}", 0.5r);
	YIO_TEST_EQ("0.5", "{}", 0.5k);
#endif

#ifdef MY_HAS_FLOAT16
	YIO_TEST_EQ("1", "{}", (_Float16)1.0);
#endif
#ifdef MY_HAS_FLOAT32
	YIO_TEST_EQ("1", "{}", (_Float32)1.0);
#endif
#ifdef MY_HAS_FLOAT64
	YIO_TEST_EQ("1", "{}", (_Float64)1.0);
#endif
#ifdef MY_HAS_FLOAT128
	YIO_TEST_EQ("1", "{}", (_Float128)1.0);
#endif
#ifdef MY_HAS_FLOAT32X
	YIO_TEST_EQ("1", "{}", (_Float32x)1.0);
#endif
#ifdef MY_HAS_FLOAT64X
	YIO_TEST_EQ("1", "{}", (_Float64x)1.0);
#endif
#ifdef MY_HAS_FLOAT128X
	YIO_TEST_EQ("1", "{}", (_Float128x)1.0);
#endif

#ifdef MY_HAS_FLOAT128_TYPE
	YIO_TEST_EQ("1", "{}", (__float128)1.0);
#endif
#ifdef MY_HAS_FLOAT80_TYPE
	YIO_TEST_EQ("1", "{}", (__float80)1.0);
#endif

#ifdef MY_HAS_DECIMAL32
	YIO_TEST_EQ("1", "{}", (_Decimal32)1.0);
#endif
#ifdef MY_HAS_DECIMAL64
	YIO_TEST_EQ("1", "{}", (_Decimal64)1.0);
#endif
#ifdef MY_HAS_DECIMAL128
	YIO_TEST_EQ("1", "{}", (_Decimal128)1.0);
#endif

#ifdef MY_HAS_COMPLEX
	YIO_TEST_EQ("1+2i", "{}", 1.0f + 2.0f * I);
	YIO_TEST_EQ("1+2i", "{}", 1.0 + 2.0 * I);
	YIO_TEST_EQ("1+2i", "{}", 1.0L + 2.0 * I);
#endif

#ifdef MY_HAS_IMAGINARY
	/* Note: Standard I might be imaginary or complex depending on implementation. */
	YIO_TEST_EQ("0+2i", "{}", 2.0f * _Imaginary_I);
	YIO_TEST_EQ("0+2i", "{}", 2.0 * _Imaginary_I);
	YIO_TEST_EQ("0+2i", "{}", 2.0L * _Imaginary_I);
#endif

	/* Wchars and Uchars - use strings as yio supports them via _Generic */
	YIO_TEST_EQ("A", "{}", L"A");
#if defined(MY_HAS_UCHAR_H) || defined(__STDC_UTF_16__) || defined(__clang__) || defined(__GNUC__)
	YIO_TEST_EQ("A", "{}", u"A");
#endif
#if defined(MY_HAS_UCHAR_H) || defined(__STDC_UTF_32__) || defined(__clang__) || defined(__GNUC__)
	YIO_TEST_EQ("A", "{}", U"A");
#endif



	/* Time types */
	struct timespec ts = { .tv_sec = 1, .tv_nsec = 500000000 };
	YIO_TEST_EQ("0:00:01.500000000", "{}", ts);

	struct timeval tv = { .tv_sec = 1, .tv_usec = 500000 };
	YIO_TEST_EQ("0:00:01.500000", "{}", tv);

	return 0;
#endif
}
