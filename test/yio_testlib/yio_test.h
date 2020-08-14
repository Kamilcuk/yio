
#pragma once
#include <yio.h>
#if _yIO_HAS_WCHAR_H
#include <ywio.h>
#endif
#if _yIO_HAS_UCHAR_H
#include <yc16io.h>
#include <yuio.h>
#endif
#include <ctype.h>
#if _yIO_HAS_UNISTRING
#include <unistr.h>
#include <unistdio.h>
#endif
#include <stdbool.h>

#ifndef _yIO_VERBOSE
#define _yIO_VERBOSE 0
#endif

enum {
	_yIO_TEST_FLAG_NOFAIL = 1,
};

_yIO_nn()
void _yIO__testing(bool verbose, const char *expr, const char *file, int line);

_yIO_format(__printf__, 5, 6) _yIO_nn()
void _yIO__test_failed(int flags, const char *expr, const char *file, int line, const char *fmt, ...);

_yIO_wur
bool _yIO_test_is_in_valgrind(void);

#define _yIO_IN_TEST_MSG(flag, expr, str, ...)  \
		(_yIO__testing(_yIO_VERBOSE, #expr, __FILE__, __LINE__), !(expr)) \
				? (_yIO__test_failed(flag, #expr, __FILE__, __LINE__, str, ##__VA_ARGS__), false) : (true)

#define _yIO_IN_TEST_1(flag, expr, ...)  _yIO_IN_TEST_MSG(flag, expr, " ")
#define _yIO_IN_TEST_2(flag, expr, ...)  _yIO_IN_TEST_MSG(flag, expr, __VA_ARGS__)
#define _yIO_IN_TEST_N(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...)  _yIO_IN_TEST_##N
#define _yIO_IN_TEST(flag, expr, ...)  _yIO_IN_TEST_N(__VA_ARGS__, 2,2,2,2,2,2,2,2,1)(flag, expr, __VA_ARGS__)

/**
 * @def _yIO_TEST(expr, ...)
 * @param expr Expression to test
 * @param ...  printf-like arguments to print
 *
 * If expr fails, then an error message is printed and
 * process will exit with EXIT_FAILURE.
 */
#define _yIO_TEST(expr, ...)  _yIO_IN_TEST(0, expr, __VA_ARGS__)

/**
 * Same as _yIO_TEST but doesn't fail the process
 */
#define _yIO_TEST_NOFAIL(expr, ...)  _yIO_IN_TEST(_yIO_TEST_FLAG_NOFAIL, expr, __VA_ARGS__)

/**
 * @see _yIO_TEST
 */
#define _yIO_TEST_MSG(expr, str, ...)  _yIO_IN_TEST_MSG(0, expr, str, ##__VA_ARGS__)

#define _yIO_test_compare_printformat_in(pf1, pf2, X) \
		_yIO_TEST(pf1.X == pf2.X, "'%s' %d(%c) %d(%c)", \
				str, pf1.X, isprint((unsigned)pf1.X)?pf1.X:'?', pf2.X, isprint((unsigned)pf2.X)?pf2.X:'?')


#define _yIO_test_compare_printformat(pf1, pf2) \
		( \
		_yIO_test_compare_printformat_in(pf1, pf2, width) && \
		_yIO_test_compare_printformat_in(pf1, pf2, precision) && \
		_yIO_test_compare_printformat_in(pf1, pf2, fill) && \
		_yIO_test_compare_printformat_in(pf1, pf2, align) && \
		_yIO_test_compare_printformat_in(pf1, pf2, sign) && \
		_yIO_test_compare_printformat_in(pf1, pf2, grouping) && \
		_yIO_test_compare_printformat_in(pf1, pf2, type) && \
		_yIO_test_compare_printformat_in(pf1, pf2, hash) && \
		_yIO_TEST(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str) \
		)











