/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <yio/yio_private.h>
#include <ctype.h>
int main() {
	int a;
	_yIO_TEST(ysscanf("-12345678901234567890", "{}", &a).error == YIO_ERROR_SCAN_UNDERFLOW);
	_yIO_TEST(ysscanf("12345678901234567890", "{}", &a).error == YIO_ERROR_SCAN_OVERFLOW);
	int b;
	struct yio_scanret_s r = ysscan("123 -12345678901234567890", &b, &a);
	_yIO_TEST(b == 123);
	_yIO_TEST(r.nitem == 1);
	_yIO_TEST_MSG(r.error == YIO_ERROR_SCAN_UNDERFLOW, "%d", r.error);
	_yIO_TEST(isdigit((unsigned char)r.lastchar));
	_yIO_TEST(r.count > 5);
}
