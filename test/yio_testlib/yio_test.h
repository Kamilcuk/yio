
#pragma once
#include <yio.h>

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
		if (!(expr)) _yIO__test_failed(#expr, __FILE__, __LINE__, str, ##__VA_ARGS__)
