/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yio/yio_private.h>
#include <string.h>
int main() {
	char str[5];
	int err = ysscanf("Hello Kamil", "Hello {}", &str).error;
	_yIO_TEST_MSG(err == -ENOMEM, "%d", err);
	_yIO_TEST(strcmp(str, "Kami") == 0);
}
