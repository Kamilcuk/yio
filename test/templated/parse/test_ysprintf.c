/**
 * @file test_ysprintf.c
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yπio.h>
#include <stdint.h> // SIZE_MAX
int main() {
	Ychar buf[200];
	int err = 0;

	err = yπsprintf(buf, sizeof(buf)/sizeof(*buf), Yc("123"));
	_yIO_TEST(err > 0, "err=%s", yio_strerror(err));
	_yIO_TEST(Ystrcmp(buf, Yc("123")) == 0);

	err = yπsprintf(buf, SIZE_MAX, Yc("123"));
	_yIO_TEST(err > 0);
	_yIO_TEST(Ystrcmp(buf, Yc("123")) == 0, m4_ifdef(«_yIO_TYPE_YIO», «"%"YPRI, buf», «""») );

	err = yπsprintf(buf, 2, Yc("123"));
	_yIO_TEST(err < 0);
	_yIO_TEST(Ystrcmp(buf, Yc("1")) == 0);

	err = yπsprintf(buf, 3, Yc("123"));
	_yIO_TEST(err < 0);
	_yIO_TEST(Ystrcmp(buf, Yc("12")) == 0);

	err = yπsprintf(buf, 4, Yc("123"));
	_yIO_TEST(err > 0);
	_yIO_TEST(Ystrcmp(buf, Yc("123")) == 0);

	err = yπsprintf(buf, 1, Yc("123"));
	_yIO_TEST(err < 0);
	_yIO_TEST(Ystrcmp(buf, Yc("")) == 0);

	return 0;
}
