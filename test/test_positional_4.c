#include <yio.h>
#include <yio_test.h>

#ifndef YYIO_HAS_timeval
#error YYIO_HAS_timeval is not defined
#endif

int main(void) {
#if YYIO_HAS_timeval
	struct timeval tv1 = { .tv_sec = 3600 + 120 + 3, .tv_usec = 123456 }; // 01:02:03.123456
	struct timeval tv2 = { .tv_sec = 0, .tv_usec = 987654 };             // 00:00:00.987654

	/* Basic positional index tests */
	YIO_TEST("1:02:03.123456 0:00:00.987654", "{0} {1}", tv1, tv2);
	YIO_TEST("0:00:00.987654 1:02:03.123456", "{1} {0}", tv1, tv2);
	
	/* Multiple arguments with timeval */
	YIO_TEST("0:00:00.987654 A 1:02:03.123456 B", "{2} {0} {3} {1}", "A", "B", tv2, tv1);
	YIO_TEST("B A 1:02:03.123456 0:00:00.987654 1:02:03.123456", "{1} {0} {3} {2} {3}", "A", "B", tv2, tv1);

	/* Formatting rules: 'f' and 'g' */
	YIO_TEST("3723.123456 0.987654", "{0:f} {1:f}", tv1, tv2);
	YIO_TEST("0.987654 3723.123456", "{1:g} {0:g}", tv1, tv2);

	/* Dynamic precision using positional indices */
	YIO_TEST("1:02:03.123 0:00:00.987", "{0:.{2}} {1:.{2}}", tv1, tv2, 3);
	YIO_TEST("1:02:03.12 0:00:00.9", "{2:.{0}} {3:.{1}}", 2, 1, tv1, tv2);
	
	/* Width and precision with positional arguments */
	YIO_TEST("3723.1      ", "{2:{0}.{1}f}", 12, 1, tv1);
	YIO_TEST("0:00:00.98   ", "{2:{0}.{1}}", 13, 2, tv2);
	
	/* 5+ Arguments mixing types, dynamic width/precision */
	YIO_TEST(
		"start 1:02:03.123456 mid 0:00:00.987654 end", 
		"{0} {1} {2} {3} {4}", 
		"start", tv1, "mid", tv2, "end"
	);

	YIO_TEST(
		"end 0:00:00.98 mid 3723.1 start", 
		"{4} {3:.{5}} {2} {1:{6}.{7}f} {0}", 
		"start", tv1, "mid", tv2, "end", 2, 6, 1
	);

	/* 5+ Args Complex reordering */
	YIO_TEST(
		"3723.123456 0:00:00.987654 1:02:03.123456 start mid end 0.987654", 
		"{1:f} {3} {1} {0} {2} {4} {3:g}", 
		"start", tv1, "mid", tv2, "end"
	);

	return 0;
#else
	return EXIT_SKIP;
#endif
}
