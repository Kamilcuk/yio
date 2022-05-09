/*
 * yio_test_float_negative.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
int main() {
	int err = 0;
	err |= yπprintf(TC("{:f}\n"), -1.0);
	err |= yπprintf(TC("{:-.0f}\n"), -1.0);
	err |= yπprintf(TC("{: .3f}\n"), -1.0);
	err |= yπprintf(TC("{: .3f}\n"), 1.0);
	err |= yπprintf(TC("{: #.1f}\n"), -1.0);
	return err;
// PASS_REGULAR_EXPRESSION -1\.000000
// PASS_REGULAR_EXPRESSION -1
// PASS_REGULAR_EXPRESSION -1\.000
// PASS_REGULAR_EXPRESSION [ ]1\.000
// PASS_REGULAR_EXPRESSION -1\.0
}
