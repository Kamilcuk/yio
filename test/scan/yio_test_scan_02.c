/*
 * yio_test_scan_01.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Kamil
 */
#include <yio.h>
int main() {
	int v;
	int err = ysscanf("1", "{}", &v).error;
	if (err) return err;
	yprint(v);
}
// PASS_REGULAR_EXPRESSION 1
