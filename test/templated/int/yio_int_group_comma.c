/*
 * yio,int,group.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
int main() {
	yπprintf(TC("{:,}\n"), 1);
	yπprintf(TC("{:,}\n"), 10);
	yπprintf(TC("{:,}\n"), 100);
	yπprintf(TC("{:,}\n"), 1000);
	yπprintf(TC("{:,}\n"), 10000);
	yπprintf(TC("{:,}\n"), 100000);
	yπprintf(TC("{:,}\n"), 1000000);
	yπprintf(TC("{:,}\n"), 10000000);
	yπprintf(TC("{:,}\n"), 100000000);
	yπprintf(TC("{:,}\n"), 1000000000);
	yπprintf(TC("{:,}\n"), 10000000000);
	yπprintf(TC("{:,}\n"), 100000000000);
	yπprintf(TC("{:,}\n"), 1000000000000);
	yπprintf(TC("{:,}\n"), 10000000000000);
	yπprintf(TC("{:,}\n"), 100000000000000);
	yπprintf(TC("{:,}\n"), 1000000000000000);
	yπprintf(TC("{:,}\n"), 10000000000000000);
	yπprintf(TC("{:,}\n"), 100000000000000000);
	yπprintf(TC("{:,}\n"), 1000000000000000000);
}
// PASS_REGULAR_EXPRESSION 1
// PASS_REGULAR_EXPRESSION 10
// PASS_REGULAR_EXPRESSION 100
// PASS_REGULAR_EXPRESSION 1,000
// PASS_REGULAR_EXPRESSION 10,000
// PASS_REGULAR_EXPRESSION 100,000
// PASS_REGULAR_EXPRESSION 1,000,000
// PASS_REGULAR_EXPRESSION 10,000,000
// PASS_REGULAR_EXPRESSION 100,000,000
// PASS_REGULAR_EXPRESSION 1,000,000,000
// PASS_REGULAR_EXPRESSION 10,000,000,000
// PASS_REGULAR_EXPRESSION 100,000,000,000
// PASS_REGULAR_EXPRESSION 1,000,000,000,000
// PASS_REGULAR_EXPRESSION 10,000,000,000,000
// PASS_REGULAR_EXPRESSION 100,000,000,000,000
// PASS_REGULAR_EXPRESSION 1,000,000,000,000,000
// PASS_REGULAR_EXPRESSION 10,000,000,000,000,000
// PASS_REGULAR_EXPRESSION 100,000,000,000,000,000
// PASS_REGULAR_EXPRESSION 1,000,000,000,000,000,000

