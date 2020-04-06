/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
int main() {
	return ysscanf("1\n", "{*}\n", (int*)0).error;
}
