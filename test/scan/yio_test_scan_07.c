/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <string.h>
int main() {
	char str[5];
	int err = ysscanf("Hello Kamil", "Hello {}", yswidth(sizeof(str) - 1), &str).error;
	yprint(err, "\n");
	yprint(str, "\n");
	if (err != 0) return -1;
	if (strcmp(str, "Kami") != 0) return -1;
}
