#ifndef SSTEST_H_
#define SSTEST_H_

#include <stdbool.h>

#ifndef SSTEST_VERBOSE
#define SSTEST_VERBOSE 0
#endif

#ifndef SSTEST_QUIET
#define SSTEST_QUIET 0
#endif

/// Represents test location and flags.
struct sstest_pos {
	/// File.
	const char *file;
	/// Function.
	const char *func;
	/// Stringified line.
	const char *line;
	/// Stringified expression.
	const char *expr;
	/// This test is not a failure.
	bool warn;
	/// This test will call abort();
	bool fail;
	/// Do not print anything in case of success.
	bool quiet;
	/// Also print something before the test.
	bool verbose;
};

#define SSTEST_STRING(a) #a
#define SSTEST_XSTRING(a) SSTEST_STRING(a)

/// Generate sstest_pos struct pointer.
#define SSTEST_GET_POS(initexpr, exprstr) \
	&(const struct sstest_pos) { \
		__FILE__, __func__, SSTEST_XSTRING(__LINE__), \
		exprstr, \
		.verbose = SSTEST_VERBOSE, \
		.quiet = SSTEST_QUIET, \
		SSTEST_POST_1 initexpr \
	}

/// Printf format string used for missing format given by user.
#define SSTEST_POST_0() "\377"
#define SSTEST_POST_1(...) __VA_ARGS__
#define SSTEST_POST_N(_9,_8,_7,_6,_5,_4,_3,_2,_1,_0, N, ...)  \
		SSTEST_POST_##N

/// @param initexpr Designator initialization expression for struct sstest_pos inside brackets.
/// @param exprstr Strigified expression.
/// @param expr The expression to execute to test.
/// @param ... Optional printf format string followed by arguments.
/// @return The result of expr cast to bool.
#define SSTEST_EX(initexpr, exprstr, expr, ...) \
	(sstest_pre(SSTEST_GET_POS(initexpr, exprstr)), \
	 sstest_post(SSTEST_GET_POS(initexpr, exprstr), !!(expr), \
		     SSTEST_POST_N(0, ##__VA_ARGS__, 1,1,1,1,1,1,1,1,1,0)(__VA_ARGS__)))

/// @param expr The expression to execute to test.
/// @param ... Optional printf format string followed by arguments.
/// @return The result of expr cast to bool.
#define SSTEST(expr, ...) SSTEST_EX((), #expr, (expr), ##__VA_ARGS__)

/// The same as SSTEST, but only a warning.
/// @see SSTEST
#define SSTEST_WARN(expr, ...) SSTEST_EX((.warn = 1), #expr, (expr), ##__VA_ARGS__)

/// The same as SSTEST, but exit on failure.
/// @see SSTEST
#define SSTEST_ASSERT(expr, ...) SSTEST_EX((.fail = 1), #expr, (expr), ##__VA_ARGS__)

#ifdef __GNUC__
__attribute__((__nonnull__))
#endif
/// Print stuff before running the test.
void sstest_pre(const struct sstest_pos *p);

/// Print stuff after running the test. So most of it.
#ifdef __GNUC__
__attribute__((__nonnull__(1, 3)))
__attribute__((__format__(__printf__, 3, 4)))
#endif
bool sstest_post(const struct sstest_pos *p, bool result, const char *fmt, ...);

#ifdef __GNUC__
__attribute__((__const__))
__attribute__((__warn_unused_result__))
#endif
bool sstest_is_in_valgrind(void);

#endif
