
#ifndef _yIO_YIO_TEST_H_
#define _yIO_YIO_TEST_H_
#include <yio.h>
#ifndef _yIO_HAS_WCHAR_H
#error _yIO_HAS_WCHAR_H
#endif
#if _yIO_HAS_WCHAR_H
#include <ywio.h>
#endif
#ifndef _yIO_HAS_UCHAR_H
#error _yIO_HAS_UCHAR_H
#endif
#if _yIO_HAS_UCHAR_H
#include <yc16io.h>
#include <yuio.h>
#endif
#include <ctype.h>
#ifndef _yIO_HAS_UNISTRING
#error
#endif
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
	_yIO_TEST_FLAG_ASSERT = 2,
};

_yIO_nn()
int _yIO__testing(bool verbose, const char *expr, const char *file, int line);

/**
 * Check if a test failed
 * @param unused Unused argument so that _yIO__testing doesn't cause a warning
 * @param result The result of the check
 * @param flags ORed _yIO_TEST_FLAG_ flags
 * @param expr The stringified expression
 * @param file
 * @param line
 * @param fmt Printf like
 * @return True if result failed, false otherwise.
 */
_yIO_format(__printf__, 7, 8) _yIO_nn(4, 5, 7)
bool _yIO__test_failed(int unused, bool result, int flags,
		const char *expr, const char *file, int line, const char *fmt, ...);

_yIO_wur
bool _yIO_test_is_in_valgrind(void);

#define _yIO_IN_TEST_MSG(flag, expr, exprstr, str, ...)  \
		_yIO__test_failed( \
				_yIO__testing(_yIO_VERBOSE, exprstr, __FILE__, __LINE__), \
				!!(expr), \
				flag, exprstr, __FILE__, __LINE__, str, ##__VA_ARGS__)

#define _yIO_IN_TEST_1(flag, expr, exprstr, ...)  \
	_yIO_IN_TEST_MSG(flag, expr, exprstr, " ")
#define _yIO_IN_TEST_2(flag, expr, exprstr, ...)  \
	_yIO_IN_TEST_MSG(flag, expr, exprstr, __VA_ARGS__)
#define _yIO_IN_TEST_N(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...)  _yIO_IN_TEST_##N
#define _yIO_IN_TEST(flag, expr, exprstr, ...)  \
	_yIO_IN_TEST_N(0, ##__VA_ARGS__, 2,2,2,2,2,2,2,2,1)(flag, expr, exprstr, ##__VA_ARGS__)

/**
 * @def _yIO_TEST(expr, ...)
 * @param expr Expression to test
 * @param ...  printf-like arguments to print
 *
 * If expr fails, then an error message is printed and
 * process will exit with EXIT_FAILURE.
 */
#define _yIO_TEST(expr, ...)  _yIO_IN_TEST(0, expr, #expr, ##__VA_ARGS__)

/**
 * Same as _yIO_TEST but doesn't fail the process
 */
#define _yIO_TEST_NOFAIL(expr, ...)  _yIO_IN_TEST(_yIO_TEST_FLAG_NOFAIL, expr, #expr, ##__VA_ARGS__)

/**
 * Same as _yIO_TEST but terminates immiedately in case of failure.
 */
#define _yIO_ASSERT(expr, ...)  _yIO_IN_TEST(_yIO_TEST_FLAG_ASSERT, expr, #expr, ##__VA_ARGS__)

/**
 * @see _yIO_TEST
 */
#define _yIO_TEST_MSG(expr, str, ...)  _yIO_IN_TEST_MSG(0, expr, #expr, str, ##__VA_ARGS__)

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

#define ARRAY_SIZE(a)  (sizeof(a)/sizeof(*a))
#define ARRAY_END(a)   (a + ARRAY_SIZE(a))
#define STRING(a)   #a
#define XSTRING(a)  STRING(a)

#define YIO_TEST(shouldbe, fmt, ...)  \
	do { \
		char buf[100]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		_yIO_TEST(err > 0, "fmt=`%s` err=%s", fmt, yio_strerror(err)); \
		_yIO_TEST(strcmp(shouldbe, buf) == 0, "shouldbe=`%s` buf=`%s`", shouldbe, buf); \
		printf("%d: `%s`\n", __LINE__, buf); \
	} while(0)

#define YIO_TEST_FAIL(fmt, ...)  \
	do { \
		char buf[100]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		_yIO_TEST(err < 0, "fmt=`%s` err=%s", fmt, yio_strerror(err)); \
	} while(0)

#endif // _yIO_YIO_TEST_H_

