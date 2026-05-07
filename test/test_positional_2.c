#include <yio_test.h>
#include <time.h>
#include <locale.h>

int main(void) {
	const struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	const struct tm t2 = {
		.tm_year = 113,
		.tm_mon  = 10,
		.tm_mday = 10,
		.tm_hour = 9,
		.tm_min  = 11,
		.tm_sec  = 21,
	};
	setlocale(LC_ALL, "C");

	/* static precision, 5 arguments, all used positionally */
	YIO_TEST("arg0 arg1 20 arg3 arg4", "{0} {1} {2:.2%Y-%m-%d %H:%M:%S} {3} {4}", "arg0", "arg1", t, "arg3", "arg4");

	/* dynamic precision, 5 arguments, all used positionally */
	YIO_TEST("arg0 arg1 2012 4 arg4", "{0} {1} {2:.{3}%Y-%m-%d %H:%M:%S} {3} {4}", "arg0", "arg1", t, 4, "arg4");

	/* dynamic precision with index 0 and value index 4, 5 arguments, all used positionally */
	YIO_TEST("7 arg1 arg2 arg3 2012-10", "{0} {1} {2} {3} {4:.{0}%Y-%m-%d %H:%M:%S}", 7, "arg1", "arg2", "arg3", t);

	/* multiple struct tm printing */
	YIO_TEST("2012 2013-11 2012-10", "{0:.4%Y} {1:.7%Y-%m} {0:.7%Y-%m}", t, t2);
	YIO_TEST("2013-11 2012-10-09", "{1:.7%Y-%m} {0:.10%Y-%m-%d}", t, t2);

	return 0;
}
