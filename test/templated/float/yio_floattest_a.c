/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
int main() {
	return yπprintf(Yc("{:a}\n"), 123.456) <= 0;
}
// PASS_REGULAR_EXPRESSION 0x
