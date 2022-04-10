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
	Ychar *buf;
	buf = yπformatf(Yc("{:+0{}}"), 3, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(Ystrcmp(buf, Yc("+000000003")) == 0);
	free(buf);
	buf = yπformatf(Yc("{:+0.{}}"), 3, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(Ystrcmp(buf, Yc("+3")) == 0);
	free(buf);
	buf = yπformatf(Yc("{:+0{}.{}}"), 3, 10, 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(Ystrcmp(buf, Yc("+000000003")) == 0);
	free(buf);
	buf = yπformatf(Yc("{:+0.{}}"), Yc("12345678901234567890"), 10);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(Ystrcmp(buf, Yc("1234567890")) == 0
			{% if MODEX == 1 %}, "%s", buf{%endif %}
	);
	free(buf);
	return 0;
}
