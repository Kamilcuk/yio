/**
 * @file int_args.c
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
int main() {
	char *buf;
	buf = yformatf("{:+0{}}", 10, 3);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(strcmp(buf, "+000000003") == 0);
	free(buf);
	buf = yformatf("{:+0.{}}", 10, 3);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(strcmp(buf, "+3") == 0);
	free(buf);
	buf = yformatf("{:+0{}.{}}", 10, 10, 3);
	_yIO_TEST(buf != NULL);
	_yIO_TEST(strcmp(buf, "+000000003") == 0);
	free(buf);
	buf = yformatf("{:+0.{}}", 10, "12345678901234567890");
	_yIO_TEST(buf != NULL);
	_yIO_TEST(strcmp(buf, "1234567890") == 0, "%s", buf);
	free(buf);
	return 0;
}
