
#pragma once
#include <yio.h>
#include <ctype.h>

#ifndef _yIO_VERBOSE
#define _yIO_VERBOSE 0
#endif

_yIO_nn()
void _yIO__testing(bool verbose, const char *expr, const char *file, int line);

_yIO_format(__printf__, 4, 5)  _yIO_nn()
void _yIO__test_failed(const char *expr, const char *file, int line, const char *fmt, ...);

/**
 * @def _yIO_TEST(expr, ...)
 * @param expr Expression to test
 * @param ...  printf-like arguments to print
 *
 * If expr fails, then an error message is printed and
 * process will exit with EXIT_FAILURE.
 */
#define _yIO_TEST_1(expr, ...)  _yIO_TEST_MSG(expr, " ")
#define _yIO_TEST_2(expr, ...)  _yIO_TEST_MSG(expr, __VA_ARGS__)
#define _yIO_TEST_N(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...)  _yIO_TEST_##N
#define _yIO_TEST(expr, ...)  _yIO_TEST_N(__VA_ARGS__, 2,2,2,2,2,2,2,2,1)(expr, __VA_ARGS__)

/**
 * @see _yIO_TEST
 */
#define _yIO_TEST_MSG(expr, str, ...)  \
		(_yIO__testing(_yIO_VERBOSE, #expr, __FILE__, __LINE__), !(expr)) \
				? (_yIO__test_failed(#expr, __FILE__, __LINE__, str, ##__VA_ARGS__), false) : (true)

#define _yIO_test_compare_printformat_in(pf1, pf2, X) \
		_yIO_TEST(pf1.X == pf2.X, "'%s' %d(%c) %d(%c)", \
				str, pf1.X, isprint(pf1.X)?pf1.X:'?', pf2.X, isprint(pf2.X)?pf2.X:'?')


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











