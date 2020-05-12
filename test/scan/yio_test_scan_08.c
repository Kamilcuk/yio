/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio_test_private.h>
#include <string.h>
int main() {
	char str[200];
	int err = ysscan("first line\nsecond line\n",
			yscan_except("\n", str), "\n").error;
	_yIO_TEST(err == 0);
	_yIO_TEST(strcmp(str, "first line") == 0);
}
