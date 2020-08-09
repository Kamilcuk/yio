/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <ywio.h>
int main() {
	int v;
	int err = ywsscanf("1", "{}", &v).error;
	if (err) return err;
	yprint(v);
}
// PASS_REGULAR_EXPRESSION 1
