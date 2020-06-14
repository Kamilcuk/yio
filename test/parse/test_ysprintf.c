/**
 * @file test_ysprintf.c
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <stdint.h>
int main() {
	char buf[200];
	int err = 0;

	err = ysprintf(buf, sizeof(buf), "123");
	_yIO_TEST(err > 0, "err=%s", yio_strerror(err));
	_yIO_TEST(strcmp(buf, "123") == 0);

	err = ysprintf(buf, SIZE_MAX, "123");
	_yIO_TEST(err > 0);
	_yIO_TEST(strcmp(buf, "123") == 0, "%s", buf);

	err = ysprintf(buf, 2, "123");
	_yIO_TEST(err < 0);
	_yIO_TEST(strcmp(buf, "1") == 0);

	err = ysprintf(buf, 3, "123");
	_yIO_TEST(err < 0);
	_yIO_TEST(strcmp(buf, "12") == 0);

	err = ysprintf(buf, 4, "123");
	_yIO_TEST(err > 0);
	_yIO_TEST(strcmp(buf, "123") == 0);

	err = ysprintf(buf, 1, "123");
	_yIO_TEST(err < 0);
	_yIO_TEST(strcmp(buf, "") == 0);

	return 0;
}
