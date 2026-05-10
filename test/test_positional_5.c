#include <yio.h>
#include <yio_test.h>
#include <time.h>

int main(void) {
#if YYIO_HAS_timeval
	const struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	struct timeval tv = { .tv_sec = 3600 + 120 + 3, .tv_usec = 123456 }; // 1:02:03.123456

	/* 
	   We mix the arguments to ensure the library handles state perfectly regardless of type placement.
	   Args (0 to 19):
	   0: 1
	   1: 2
	   2: t
	   3: 3
	   4: yio_repr_oct("a\0011")
	   5: 4
	   6: tv
	   7: 5
	   8: "one"
	   9: 6
	   10: yio_repr_hex("a\0011")
	   11: 7
	   12: "two"
	   13: 8
	   14: yio_repr_caret("a\001")
	   15: 9
	   16: 10
	   17: 11
	   18: 12
	   19: 13
	*/

#define TEST_ARGS \
	1, 2, t, 3, yio_repr_oct("a\0011"), 4, tv, 5, "one", 6, \
	yio_repr_hex("a\0011"), 7, "two", 8, yio_repr_caret("a\001"), 9, 10, 11, 12, 13

	/* Sequential Arguments */
	YIO_TEST(
		"1 2 2012 3 a\\0011 4 1:02:03.123456 5 one 6 a\\x011 7 two 8 a^A 9 10 11 12 13", 
		"{} {} {:%Y} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", 
		TEST_ARGS
	);

	/* Positional Arguments with mixed order and repetitions */
	YIO_TEST(
		"13 a^A one a\\0011 2012-10-09 1:02:03.123456 a\\x011 two 1", 
		"{19} {14} {8} {4} {2:%Y-%m-%d} {6} {10} {12} {0}", 
		TEST_ARGS
	);

	/* Dynamic Precision */
	/* {0}=1, {12}=two -> prec 1 -> t */
	/* {7}=5, {8}=one -> prec 5 -> one */
	/* {3}=3, {6}=tv -> prec 3 -> 1:02:03.123 */
	/* {5}=4, {2}=t -> prec 4 -> %Y is 2012 */
	YIO_TEST(
		"1 t one 1:02:03.123 2012", 
		"{0} {12:.{0}} {8:.{7}} {6:.{3}} {2:.{5}%Y}", 
		TEST_ARGS
	);

	/* Self-referential width/precision (where it makes sense, integers) */
	/* {1}=2, {0}=1 -> width 2 -> " 1" */
	/* {5}=4, {3}=3 -> width 4 -> "   3" */
	YIO_TEST(
		" 1    3", 
		"{0:{1}} {3:{5}}", 
		TEST_ARGS
	);

	/* Extreme long string testing repeated jumping across indices to ensure no context leaking */
	YIO_TEST(
		"two 1 one 2 a\\0011 2012-10 1:02:03.12 a\\x011 a^A",
		"{12} {0} {8} {1} {4} {2:%Y-%m} {6:.{1}} {10} {14}",
		TEST_ARGS
	);

	return 0;
#else
	return EXIT_SKIP;
#endif
}
