/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <ywio.h>
int main() {
	int *a;
	return ywsscanf(L"1", L"{;*}\n", a);
}
