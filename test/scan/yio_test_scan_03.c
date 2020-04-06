/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yio/yio_private.h>
int main() {
	char str[20];
	int err = ysscanf("Hello Kamil", "Hello {}", &str).error;
	if (err) return err;
	_yIO_TEST(strcmp(str, "Kamil") == 0);
	return 0;
}
// // PASS_REGULAR_EXPRESSION Kamil
