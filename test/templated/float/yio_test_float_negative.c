/*
 * yio_test_float_negative.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
int main() {
	yπprintf(TC("{:f}\n"), -1.0);
	yπprintf(TC("{:-.0f}\n"), -1.0);
	yπprintf(TC("{: .3f}\n"), -1.0);
	yπprintf(TC("{: .3f}\n"), 1.0);
	yπprintf(TC("{: #.1f}\n"), -1.0);
}
// PASS_REGULAR_EXPRESSION -1\.[0-9]+
// PASS_REGULAR_EXPRESSION -1
