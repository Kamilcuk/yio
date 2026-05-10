#ifndef YYIO_TEST_SDCC_CONFTEST_H_
#define YYIO_TEST_SDCC_CONFTEST_H_

#include <yio.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#if !defined(__SDCC)
#define libtest_main main
#endif

#if defined(__SDCC)
#define atexit(...)
#define fflush(...)
#define fprintf(stream, ...) printf(__VA_ARGS__)
#define vfprintf(stream, fmt, va) vprintf(fmt, va)
#define _Exit(code) exit_test(code)
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define YIO_TEST_FLAG_NOFAIL 0x01
#define YIO_TEST_FLAG_ASSERT 0x02

void abort(void);
void exit_test(int code);

#define YIO_TESTEXPR(e, ...)     do { \
	if (!(e)) { \
		printf("ERROR: %s:%s:%d: ", __FILE__, __func__, __LINE__); \
		__VA_OPT__(printf(__VA_ARGS__);) \
		printf("\n"); \
		abort(); \
	} \
} while(0)
#define YIO_TESTEXPR_NOFAIL      YIO_TESTEXPR
#define YIO_TESTEXPR_ASSERT      YIO_TESTEXPR
#define YYIO_test_is_in_valgrind 0
#else
#include <sstest.h>
#define YIO_TESTEXPR             SSTEST
#define YIO_TESTEXPR_NOFAIL      SSTEST_WARN
#define YIO_TESTEXPR_ASSERT      SSTEST_ASSERT
#define YYIO_test_is_in_valgrind sstest_is_in_valgrind
#endif

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

#define in_YIO_TEST(shouldbe, fmt, buf, err, ...)  do { \
		const struct testparam testparam = { \
			BRACES(shouldbe, \
				shouldbe \
			, \
				ESC shouldbe \
			) \
		}; \
		if (testparam.fail) { \
			YIO_TESTEXPR((err) < 0, "should fail fmt=`%s` err=%d`%s`", \
					fmt, err, yio_strerror(err)); \
		} else if (testparam.errnum) { \
			YIO_TESTEXPR((err) != testparam.errnum, "should fail with %d`%s` fmt=`%s` err=%d`%s`", \
					testparam.errnum, yio_strerror(testparam.errnum), \
					fmt, err, yio_strerror(err)); \
		} else { \
			YIO_TESTEXPR((err) > 0, "failed fmt=`%s` err=%d`%s`", \
					fmt, err, yio_strerror(err)); \
		} \
		if (testparam.ret) { \
			YIO_TESTEXPR((err) == testparam.ret, "ret_not_ok fmt=`%s` ret=%d err=%d`%s`", \
					fmt, testparam.ret, err, yio_strerror(err)); \
		} \
		if (testparam.eq) { \
			YIO_TESTEXPR(!strcmp(testparam.eq, buf), \
					"not equal testparam.eq=\n\t`%s` buf=\n\t`%s`", testparam.eq, buf); \
		} \
	} while (0)

#define YIO_TEST(shouldbe, fmt, ...)  do { \
		char buf[128]; \
		const int err = yio_snprintf(buf, sizeof(buf), fmt, ## __VA_ARGS__); \
		in_YIO_TEST(shouldbe, fmt, buf, err); \
	} while(0)


#define YIO_TEST_FAIL(fmt, ...)     YIO_TEST(( .fail=1 ), fmt, ##__VA_ARGS__)
#define YIO_TEST_EQ(EQ, ...)     YIO_TEST((.eq = EQ), __VA_ARGS__)

#endif // YYIO_TEST_SDCC_CONFTEST_H_
