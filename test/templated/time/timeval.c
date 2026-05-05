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
#if YYIO_HAS_timeval
	struct timeval t = {
		.tv_sec = 1234567,
		.tv_usec = 123456,
	};
	YIO_TEST_EQ("342:56:07.123456", "{}", t);
	YIO_TEST_EQ("1234567.123456", "{:f}", t);
	YIO_TEST_EQ("1234567.12", "{:.2f}", t);
	YIO_TEST_EQ("1234567.123456", "{:g}", t);
	YIO_TEST_EQ("342:56:07.123456", "{:#}", t);
	
	t.tv_usec = 100000;
	YIO_TEST_EQ("1234567.1", "{:g}", t);
	YIO_TEST_EQ("342:56:07.1", "{:#}", t);

	t.tv_usec = 0;
	YIO_TEST_EQ("1234567", "{:g}", t);
	YIO_TEST_EQ("342:56:07", "{:#}", t);

	struct timeval t2 = { .tv_sec = 1, .tv_usec = 1 };
	YIO_TEST_EQ("0:00:01.000001", "{}", t2);
	YIO_TEST_EQ("1.000001", "{:f}", t2);

	struct timeval tn = { .tv_sec = -1, .tv_usec = 0 };
	YIO_TEST_EQ("-0:00:01.000000", "{}", tn);
	return 0;
#else
	return 77;
#endif
}
