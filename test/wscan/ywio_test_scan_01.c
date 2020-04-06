/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <ywio.h>
int main() {
	return yswscanf(L"1", L"{;*}\n", int*);
}
