
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
#include <sstest.h>

#define YIO_TESTEXPR             SSTEST
#define YIO_TESTEXPR_NOFAIL      SSTEST_WARN
#define YIO_TESTEXPR_ASSERT      SSTEST_ASSERT
#define YYIO_test_is_in_valgrind sstest_is_in_valgrind

#define YYIO_test_compare_printformat_in(pf1, pf2, X) \
		YIO_TESTEXPR(pf1.X == pf2.X, "'%s' %d(%c) %d(%c)", \
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
		YIO_TESTEXPR(memcmp(&pf, &shouldbe, sizeof(pf)) == 0, "%s", str) \
		)

/* ------------------------------------------------------------------------- */

#define ARRAY_SIZE(a)  (sizeof(a)/sizeof(*a))
#define ARRAY_END(a)   (a + ARRAY_SIZE(a))
#define STRING(a)   #a
#define XSTRING(a)  STRING(a)

struct testparam {
	const char *eq;
	const char *rgx;
	int ret;
	bool fail;
	int errnum;
};

#define ESC(...)  __VA_ARGS__
#define BRACES_ESC(...)  1, 2
#define BRACES_1(_1, ...)  _1
#define BRACES_2(_1, ...)  __VA_ARGS__
#define BRACES_IN2(_0, _1, _2, N, ...)   BRACES_##N
#define BRACES_IN(arg, ...)              BRACES_IN2(0, arg, 2, 1)(__VA_ARGS__)
#define BRACES(arg, has, dont)           BRACES_IN(BRACES_ESC arg, has, dont)

#define in_YIO_TEST(shouldbe, fmt, buf, err)  do { \
		const struct testparam testparam = { \
			BRACES(shouldbe, \
				shouldbe \
			, \
				ESC shouldbe \
			) \
		}; \
		if (testparam.fail) { \
			YIO_TESTEXPR((err) < 0, "should fail fmt=`%s` err=%d`%s` errno=%d", \
					fmt, err, yio_strerror(err), errno); \
		} else if (testparam.errnum) { \
			YIO_TESTEXPR((err) != testparam.errnum, "should fail with %d`%s` fmt=`%s` err=%d`%s` errno=%d", \
					testparam.errnum, yio_strerror(testparam.errnum), \
					fmt, err, yio_strerror(err), errno); \
		} else { \
			YIO_TESTEXPR((err) > 0, "failed fmt=`%s` err=%d`%s` errno=%d", \
					fmt, err, yio_strerror(err), errno); \
		} \
		if (testparam.ret) { \
			YIO_TESTEXPR((err) == testparam.ret, "ret_not_ok fmt=`%s` ret=%d err=%d`%s` errno=%d", \
					fmt, testparam.ret, err, yio_strerror(err), errno); \
		} \
		if (testparam.rgx) { \
			int matchlen = -1; \
			char *theregex = malloc(strlen(testparam.rgx) + 3); \
			if (theregex == NULL) abort(); \
			sprintf(theregex, "^%s$", testparam.rgx); \
			YIO_TESTEXPR(re_match(theregex, buf, &matchlen) == 0, \
					"theregex=\n\t`%s` buf=\n\t`%s`", theregex, buf); \
			free(theregex); \
		} \
		if (testparam.eq) { \
			YIO_TESTEXPR(!strcmp(testparam.eq, buf), \
					"not equal testparam.eq=\n\t`%s` buf=\n\t`%s`", testparam.eq, buf); \
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

#define YIO_TEST_FAIL(fmt, ...)     YIO_TEST(( .fail=1 ), fmt, ##__VA_ARGS__)
#define YWIO_TEST_FAIL(fmt, ...)    YWIO_TEST(( .fail=1 ), fmt, ##__VA_ARGS__)
#define YC16IO_TEST_FAIL(fmt, ...)  YC16IO_TEST(( .fail=1 ), fmt, ##__VA_ARGS__)
#define YUIO_TEST_FAIL(fmt, ...)    YUIO_TEST(( .fail=1 ), fmt, ##__VA_ARGS__)

#define YIO_TEST_EQ(EQ, ...)     YIO_TEST((.eq = EQ), __VA_ARGS__)
#define YWIO_TEST_EQ(EQ, ...)    YWIO_TEST((.eq = EQ), __VA_ARGS__)
#define YC16IO_TEST_EQ(EQ, ...)  YC16IO_TEST((.eq = EQ), __VA_ARGS__)
#define YUIO_TEST_EQ(EQ, ...)    YUIO_TEST((.eq = EQ), __VA_ARGS__)

#define YIO_TEST_RGX(RGX, ...)     YIO_TEST((.rgx = RGX), __VA_ARGS__)
#define YWIO_TEST_RGX(RGX, ...)    YWIO_TEST((.rgx = RGX), __VA_ARGS__)
#define YC16IO_TEST_RGX(RGX, ...)  YC16IO_TEST((.rgx = RGX), __VA_ARGS__)
#define YUIO_TEST_RGX(RGX, ...)    YUIO_TEST((.rgx = RGX), __VA_ARGS__)

/* ------------------------------------------------------------------------- */

#endif // YYIO_YIO_TEST_H_

