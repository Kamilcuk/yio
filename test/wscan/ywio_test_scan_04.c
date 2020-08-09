/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <ywio.h>
int main() {
	wchar_t str[5];
	int err = ywsscanf(L"Hello Kamil", L"Hello {}", &str).error;
	if (err != -ENOMEM) return -1;
	ywprint(str);
}
// PASS_REGULAR_EXPRESSION Kami
