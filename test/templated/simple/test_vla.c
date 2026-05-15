/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yio.h>
int main() {
	// int i = 5;
	int a[5]; // NOLINT(runtime/arrays)
	a[0] = 5;
	a[1] = 4;
	a[2] = 3;
	a[3] = 2;
	a[4] = 1;
	yio_print("{}\n", a[0]);
	yio_print("{} {}\n", a[1], a[2]);
	yio_print("{} {} {} {} {}\n", a[3], a[4], a[0], a[1], a[2]);
	yio_stream(a[0], "\n");
	yio_stream(a[1], " ", a[2], "\n");
	yio_stream(a[3], " ", a[4], " ", a[0], " ", a[1], " ", a[2], "\n");
}
// PASS_REGULAR_EXPRESSION 5
// PASS_REGULAR_EXPRESSION 4 3
// PASS_REGULAR_EXPRESSION 2 1 5 4 3
// PASS_REGULAR_EXPRESSION 5
// PASS_REGULAR_EXPRESSION 4 3
// PASS_REGULAR_EXPRESSION 2 1 5 4 3
