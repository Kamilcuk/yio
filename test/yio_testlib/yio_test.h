
#ifndef YYIO_YIO_TEST_H_
#define YYIO_YIO_TEST_H_
#include <yio.h>
#ifndef YIO_HAS_WCHAR_H
#error YIO_HAS_WCHAR_H
#endif
#if YIO_HAS_WCHAR_H
#include <ywio.h>
#endif
#ifndef YIO_HAS_UCHAR_H
#error YIO_HAS_UCHAR_H
#endif
#if YIO_HAS_UCHAR_H
#include <yc16io.h>
#include <yuio.h>
#endif
#include <ctype.h>
#ifndef YYIO_HAS_UNISTRING
#error
#endif
#if YYIO_HAS_UNISTRING
#include <unistr.h>
#include <unistdio.h>
#endif
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <re.h> // tiny-regex-c

#ifndef YYIO_VERBOSE
#define YYIO_VERBOSE 0
#endif

enum {
	YYIO_TEST_FLAG_NOFAIL = 1,
	YYIO_TEST_FLAG_ASSERT = 2,
};

bool YYIO_tty(void);

YYIO_nn()
int YYIO__testing(bool verbose, const char *expr, const char *file, int line);

/**
 * Check if a test failed
 * @param unused Unused argument so that YYIO__testing doesn't cause a warning
 * @param result The result of the check
 * @param flags ORed YYIO_TEST_FLAG_ flags
 * @param expr The stringified expression
 * @param file
 * @param line
 * @param fmt Printf like
 * @return True if result failed, false otherwise.
 */
YYIO_format(__printf__, 7, 8) YYIO_nn(4, 5, 7)
bool YYIO__test_failed(int unused, bool result, int flags,
		const char *expr, const char *file, int line, const char *fmt, ...);

YYIO_wur
bool YYIO_test_is_in_valgrind(void);

#define YYIO_IN_TEST_MSG(flag, expr, exprstr, str, ...)  \
		YYIO__test_failed( \
				YYIO__testing(YYIO_VERBOSE, exprstr, __FILE__, __LINE__), \
				!!(expr), \
				flag, exprstr, __FILE__, __LINE__, str, ##__VA_ARGS__)

#define YYIO_IN_TEST_1(flag, expr, exprstr, ...)  \
	YYIO_IN_TEST_MSG(flag, expr, exprstr, " ")
#define YYIO_IN_TEST_2(flag, expr, exprstr, ...)  \
	YYIO_IN_TEST_MSG(flag, expr, exprstr, __VA_ARGS__)
#define YYIO_IN_TEST_N(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...)  YYIO_IN_TEST_##N
#define YYIO_IN_TEST(flag, expr, exprstr, ...)  \
	YYIO_IN_TEST_N(0, ##__VA_ARGS__, 2,2,2,2,2,2,2,2,1)(flag, expr, exprstr, ##__VA_ARGS__)

/**
 * @def YYIO_TEST(expr, ...)
 * @param expr Expression to test
 * @param ...  printf-like arguments to print
 *
 * If expr fails, then an error message is printed and
 * process will exit with EXIT_FAILURE.
 */
#define YYIO_TEST(expr, ...)  YYIO_IN_TEST(0, expr, #expr, ##__VA_ARGS__)

/**
 * Same as YYIO_TEST but doesn't fail the process
 */
#define YYIO_TEST_NOFAIL(expr, ...)  YYIO_IN_TEST(YYIO_TEST_FLAG_NOFAIL, expr, #expr, ##__VA_ARGS__)

/**
 * Same as YYIO_TEST but terminates immiedately in case of failure.
 */
#define YYIO_ASSERT(expr, ...)  YYIO_IN_TEST(YYIO_TEST_FLAG_ASSERT, expr, #expr, ##__VA_ARGS__)

/**
 * @see YYIO_TEST
 */
#define YYIO_TEST_MSG(expr, str, ...)  YYIO_IN_TEST_MSG(0, expr, #expr, str, ##__VA_ARGS__)

#define YYIO_test_compare_printformat_in(pf1, pf2, X) \
		YYIO_TEST(pf1.X == pf2.X, "'%s' %d(%c) %d(%c)", \
				str, pf1.X, isprint((unsigned)pf1.X)?pf1.X:'?', pf2.X, isprint((unsigned)pf2.X)?pf2.X:'?')


#define YYIO_test_compare_printformat(pf1, pf2) \
		( \
		YYIO_test_compare_printformat_in(pf1, pf2, width) && \
		YYIO_test_compare_printformat_in(pf1, pf2, precision) && \
		YYIO_test_compare_printformat_in(pf1, pf2, fill) && \
		YYIO_test_compare_printformat_in(pf1, pf2, align) && \
		YYIO_test_compare_printformat_in(pf1, pf2, sign) && \
		YYIO_test_compare_printformat_in(pf1, pf2, grouping) && \
		YYIO_test_compare_printformat_in(pf1, pf2, type) && \
		YYIO_test_compare_printformat_in(pf1, pf2, hash) && \
		YYIO_TEST(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str) \
		)

#define ARRAY_SIZE(a)  (sizeof(a)/sizeof(*a))
#define ARRAY_END(a)   (a + ARRAY_SIZE(a))
#define STRING(a)   #a
#define XSTRING(a)  STRING(a)

struct testparam {
	const char *rgx;
	const char *match;
	bool fail;
};

#define in_YIO_TEST(shouldbe, fmt, buf, err)  do { \
		YYIO_TEST((err) > 0, "fmt=`%s` err=%d`%s` errno=%d", \
				fmt, err, yio_strerror(err), errno); \
		int matchlen; \
		YYIO_TEST(re_match("^" shouldbe "$", buf, &matchlen) == 0, "shouldbe=\n\t`%s` buf=\n\t`%s`", shouldbe, buf); \
		if ((err) > 0 && re_match("^" shouldbe "$", buf, &matchlen) == 0) { \
			fprintf(stderr, "GOOD:%d:`%s`%s`\n", __LINE__, shouldbe, buf); \
		} \
	} while (0)

#define YIO_TEST(shouldbe, fmt, ...)  do { \
		char buf[1024]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		in_YIO_TEST(shouldbe, fmt, buf, err); \
	} while(0)


#if YIO_HAS_WCHAR_H
#define YWIO_TEST(shouldbe, fmt, ...)  do { \
		wchar_t buf[1024] = {0}; \
		const int err = ywsprintf(buf, sizeof(buf), L##fmt, ## __VA_ARGS__); \
		char bufc[1024]; \
		wcstombs(bufc, buf, sizeof(bufc)); \
		in_YIO_TEST(shouldbe, fmt, bufc, err); \
	} while(0)
#endif

#if YYIO_HAS_UNISTRING
#define YC16IO_TEST(shouldbe, fmt, ...)  do { \
		char16_t buf[1024]; \
		const int err = yc16sprintf(buf, sizeof(buf), u##fmt, ## __VA_ARGS__); \
		char bufc[1024]; \
		size_t lengthp = sizeof(bufc); \
		u16_to_u8(buf, u16_strlen(buf) + 1, (uint8_t*)bufc, &lengthp); \
		in_YIO_TEST(shouldbe, fmt, bufc, err); \
	} while(0)

#define YUIO_TEST(shouldbe, fmt, ...)  do { \
		char32_t buf[1024]; \
		const int err = yusprintf(buf, sizeof(buf), U##fmt, ## __VA_ARGS__); \
		char bufc[1024]; \
		size_t lengthp = sizeof(bufc); \
		u32_to_u8(buf, u32_strlen(buf) + 1, (uint8_t*)bufc, &lengthp); \
		in_YIO_TEST(shouldbe, fmt, bufc, err); \
	} while(0)
#endif

#define YIO_TEST_FAIL(fmt, ...)  \
	do { \
		char buf[100]; \
		const int err = ysprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		YYIO_TEST(err < 0, "fmt=`%s` err=%d`%s` buf=`%s`", fmt, err, yio_strerror(err), buf); \
	} while(0)

#endif // YYIO_YIO_TEST_H_

