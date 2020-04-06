/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <ywio.h>
int main() {
	wchar_t str[20];
	int err = ysscanf(L"Hello Kamil", L"Hello {}", &str).error;
	if (err) return err;
	ywprint(str);
}
// PASS_REGULAR_EXPRESSION Kamil
