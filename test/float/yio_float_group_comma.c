/*
 * yio,int,group.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
int main() {
	yprintf("{:,f}\n", 1.123);
	yprintf("{:,f}\n", 10.123);
	yprintf("{:,f}\n", 100.123);
	yprintf("{:,f}\n", 1000.123);
	yprintf("{:,f}\n", 10000.123);
	yprintf("{:,f}\n", 100000.123);
	yprintf("{:,f}\n", 1000000.123);
	yprintf("{:,f}\n", 10000000.123);
	yprintf("{:,f}\n", 100000000.123);
	yprintf("{:,f}\n", 1000000000.123);
	yprintf("{:,f}\n", 10000000000.123);
	yprintf("{:,f}\n", 100000000000.123);
	yprintf("{:,f}\n", 1000000000000.123);
	yprintf("{:,f}\n", 10000000000000.123);
	yprintf("{:,f}\n", 100000000000000.123);
	yprintf("{:,f}\n", 1000000000000000.123);
	yprintf("{:,f}\n", 10000000000000000.123);
	yprintf("{:,f}\n", 100000000000000000.123);
	yprintf("{:,f}\n", 1000000000000000000.123);
}
// PASS_REGULAR_EXPRESSION 1\.[0-9]+
// PASS_REGULAR_EXPRESSION 10\.[0-9]+
// PASS_REGULAR_EXPRESSION 100\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 10,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 100,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 10,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 100,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 10,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 100,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 10,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 100,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 10,000,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 100,000,000,000,000,000\.[0-9]+
// PASS_REGULAR_EXPRESSION 1,000,000,000,000,000,000\.[0-9]+

