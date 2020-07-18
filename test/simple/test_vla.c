/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

int main() {
	int i = 5;
	int a[i];
	a[0] = 5;
	a[1] = 4;
	a[2] = 3;
	a[3] = 2;
	a[4] = 1;
	yprintf("{}\n", a[0]);
	yprintf("{} {}\n", a[1], a[2]);
	yprintf("{} {} {} {} {}\n", a[3], a[4], a[0], a[1], a[2]);
	yprint(a[0], "\n");
	yprint(a[1], " ", a[2], "\n");
	yprint(a[3], " ", a[4], " ", a[0], " ", a[1], " ", a[2], "\n");
}
// PASS_REGULAR_EXPRESSION 5
// PASS_REGULAR_EXPRESSION 4 3
// PASS_REGULAR_EXPRESSION 2 1 5 4 3
// PASS_REGULAR_EXPRESSION 5
// PASS_REGULAR_EXPRESSION 4 3
// PASS_REGULAR_EXPRESSION 2 1 5 4 3
