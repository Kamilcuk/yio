/*
 * yio_test_float_negative.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
int main() {
	yprintf("{:f}\n", -1.0);
	yprintf("{:-.0f}\n", -1.0);
	yprintf("{: .3f}\n", -1.0);
	yprintf("{: .3f}\n", 1.0);
	yprintf("{: #.1f}\n", -1.0);
}
// PASS_REGULAR_EXPRESSION -1\.[0-9]+
// PASS_REGULAR_EXPRESSION -1
