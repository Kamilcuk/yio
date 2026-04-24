/*
 * yio_test_float_negative.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
int main() {
	int err = 0;
	err |= yio_printf("{:f}\n", -1.0);
	err |= yio_printf("{:-.0f}\n", -1.0);
	err |= yio_printf("{: .3f}\n", -1.0);
	err |= yio_printf("{: .3f}\n", 1.0);
	err |= yio_printf("{: #.1f}\n", -1.0);
	return err;
// PASS_REGULAR_EXPRESSION -1\.000000
// PASS_REGULAR_EXPRESSION -1
// PASS_REGULAR_EXPRESSION -1\.000
// PASS_REGULAR_EXPRESSION [ ]1\.000
// PASS_REGULAR_EXPRESSION -1\.0
}
