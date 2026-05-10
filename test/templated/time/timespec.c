/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>
#include <yio_test.h>
#include <string.h>

int main() {
#if YYIO_HAS_timespec
	struct timespec t = {
		.tv_sec = 1234567,
		.tv_nsec = 123456789,
	};
	YIO_TEST_EQ("342:56:07.123456789", "{}", t);
	YIO_TEST_EQ("1234567.123456789", "{:f}", t);
	YIO_TEST_EQ("1234567.123", "{:.3f}", t);
	YIO_TEST_EQ("1234567.123456789", "{:g}", t);
	YIO_TEST_EQ("342:56:07.123456789", "{:#}", t);
	
	t.tv_nsec = 100000000;
	YIO_TEST_EQ("1234567.1", "{:g}", t);
	YIO_TEST_EQ("342:56:07.1", "{:#}", t);

	t.tv_nsec = 0;
	YIO_TEST_EQ("1234567", "{:g}", t);
	YIO_TEST_EQ("1234567", "{:.0g}", t);
	YIO_TEST_EQ("342:56:07", "{:#}", t);

	struct timespec t2 = { .tv_sec = 1, .tv_nsec = 1 };
	YIO_TEST_EQ("0:00:01.000000001", "{}", t2);
	YIO_TEST_EQ("1.000000001", "{:f}", t2);

	struct timespec tn = { .tv_sec = -1, .tv_nsec = 0 };
	YIO_TEST_EQ("-0:00:01.000000000", "{}", tn);
	return 0;
#else
	return EXIT_SKIP;
#endif
}
