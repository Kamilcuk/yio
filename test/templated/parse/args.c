/**
 * @file int_args.c
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yπio.h>

int main() {
	TCHAR *buf;
	buf = yπformatf(TC("{:+0{}}"), 3, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(TSTRCMP(buf, TC("+000000003")) == 0);
	free(buf);
	buf = yπformatf(TC("{:+0.{}}"), 3, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(TSTRCMP(buf, TC("+3")) == 0);
	free(buf);
	buf = yπformatf(TC("{:+0{}.{}}"), 3, 10, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(TSTRCMP(buf, TC("+000000003")) == 0);
	free(buf);
	buf = yπformatf(TC("{:+0.{}}"), TC("12345678901234567890"), 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(TSTRCMP(buf, TC("1234567890")) == 0);
	free(buf);
	return 0;
}
