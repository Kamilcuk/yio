/*
 * yio_int_group.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
int main() {
	yπprintf(TC("{:_}\n"), 1);
	yπprintf(TC("{:_}\n"), 10);
	yπprintf(TC("{:_}\n"), 100);
	yπprintf(TC("{:_}\n"), 1000);
	yπprintf(TC("{:_}\n"), 10000);
	yπprintf(TC("{:_}\n"), 100000);
	yπprintf(TC("{:_}\n"), 1000000);
	yπprintf(TC("{:_}\n"), 10000000);
	yπprintf(TC("{:_}\n"), 100000000);
	yπprintf(TC("{:_}\n"), 1000000000);
	yπprintf(TC("{:_}\n"), 10000000000);
	yπprintf(TC("{:_}\n"), 100000000000);
	yπprintf(TC("{:_}\n"), 1000000000000);
	yπprintf(TC("{:_}\n"), 10000000000000);
	yπprintf(TC("{:_}\n"), 100000000000000);
	yπprintf(TC("{:_}\n"), 1000000000000000);
	yπprintf(TC("{:_}\n"), 10000000000000000);
	yπprintf(TC("{:_}\n"), 100000000000000000);
	yπprintf(TC("{:_}\n"), 1000000000000000000);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 10
// PASS_REGULAR_EXPRESSION 100
// PASS_REGULAR_EXPRESSION 1_000
// PASS_REGULAR_EXPRESSION 10_000
// PASS_REGULAR_EXPRESSION 100_000
// PASS_REGULAR_EXPRESSION 1_000_000
// PASS_REGULAR_EXPRESSION 10_000_000
// PASS_REGULAR_EXPRESSION 100_000_000
// PASS_REGULAR_EXPRESSION 1_000_000_000
// PASS_REGULAR_EXPRESSION 10_000_000_000
// PASS_REGULAR_EXPRESSION 100_000_000_000
// PASS_REGULAR_EXPRESSION 1_000_000_000_000
// PASS_REGULAR_EXPRESSION 10_000_000_000_000
// PASS_REGULAR_EXPRESSION 100_000_000_000_000
// PASS_REGULAR_EXPRESSION 1_000_000_000_000_000
// PASS_REGULAR_EXPRESSION 10_000_000_000_000_000
// PASS_REGULAR_EXPRESSION 100_000_000_000_000_000
// PASS_REGULAR_EXPRESSION 1_000_000_000_000_000_000



