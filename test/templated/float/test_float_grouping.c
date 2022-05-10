/*
 * test_grouping.c
 *
 *  Created on: 3 kwi 2020
 *      Author: kamil
 */
#include <yπio.h>
int main() {

	yπprintf(TC("{:_f}\n"), 1.125);
	yπprintf(TC("{:_f}\n"), 21.125);
	yπprintf(TC("{:_f}\n"), 321.125);
	yπprintf(TC("{:_f}\n"), 4321.125);
	yπprintf(TC("{:_f}\n"), 54321.125);
	yπprintf(TC("{:_f}\n"), 654321.125);
	yπprintf(TC("{:_f}\n"), 7654321.125);
	yπprintf(TC("{:_f}\n"), 87654321.125);
	yπprintf(TC("{:_f}\n"), 987654321.125);
	yπprintf(TC("{:_f}\n"), 1987654321.125);
	yπprintf(TC("{:_f}\n"), 21987654321.125);
	yπprintf(TC("{:_f}\n"), 321987654321.125);
	yπprintf(TC("{:_f}\n"), 4321987654321.125);
	yπprintf(TC("{:_f}\n"), 54321987654321.125);
	yπprintf(TC("{:_f}\n"), 654321987654321.125);
	yπprintf(TC("{:_f}\n"), 7654321987654321.000);

// PASS_REGULAR_EXPRESSION 1.125000
// PASS_REGULAR_EXPRESSION 21.125000
// PASS_REGULAR_EXPRESSION 321.125000
// PASS_REGULAR_EXPRESSION 4_321.125000
// PASS_REGULAR_EXPRESSION 54_321.125000
// PASS_REGULAR_EXPRESSION 654_321.125000
// PASS_REGULAR_EXPRESSION 7_654_321.125000
// PASS_REGULAR_EXPRESSION 87_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 1_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 21_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 321_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 4_321_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 54_321_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 654_321_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 7_654_321_987_654_321.[0-9][0-9][0-9][0-9][0-9][0-9]

	yπprintf(TC("{:,f}\n"), 1.125);
	yπprintf(TC("{:,f}\n"), 21.125);
	yπprintf(TC("{:,f}\n"), 321.125);
	yπprintf(TC("{:,f}\n"), 4321.125);
	yπprintf(TC("{:,f}\n"), 54321.125);
	yπprintf(TC("{:,f}\n"), 654321.125);
	yπprintf(TC("{:,f}\n"), 7654321.125);
	yπprintf(TC("{:,f}\n"), 87654321.125);
	yπprintf(TC("{:,f}\n"), 987654321.125);
	yπprintf(TC("{:,f}\n"), 1987654321.125);
	yπprintf(TC("{:,f}\n"), 21987654321.125);
	yπprintf(TC("{:,f}\n"), 321987654321.125);
	yπprintf(TC("{:,f}\n"), 4321987654321.125);
	yπprintf(TC("{:,f}\n"), 54321987654321.125);
	yπprintf(TC("{:,f}\n"), 654321987654321.125);
	yπprintf(TC("{:,f}\n"), 7654321987654321.000);

// PASS_REGULAR_EXPRESSION 1.125000
// PASS_REGULAR_EXPRESSION 21.125000
// PASS_REGULAR_EXPRESSION 321.125000
// PASS_REGULAR_EXPRESSION 4,321.125000
// PASS_REGULAR_EXPRESSION 54,321.125000
// PASS_REGULAR_EXPRESSION 654,321.125000
// PASS_REGULAR_EXPRESSION 7,654,321.125000
// PASS_REGULAR_EXPRESSION 87,654,321.125000
// PASS_REGULAR_EXPRESSION 987,654,321.125000
// PASS_REGULAR_EXPRESSION 1,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 21,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 321,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 4,321,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 54,321,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 654,321,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]
// PASS_REGULAR_EXPRESSION 7,654,321,987,654,321.[0-9][0-9][0-9][0-9][0-9][0-9]

}