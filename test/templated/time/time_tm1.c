/**
 * @file
 * @date 2020-lip-22
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>
int main() {
	struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	yprintf("{:%Y-%m-%d %H:%M:%S}\n", t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
	yprintf("{:%D}\n", t);
// PASS_REGULAR_EXPRESSION 10/09/12
	yprintf("{:%F %T}\n", t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
	yprintf("{:A%%B{{C}}D}\n", t);
// PASS_REGULAR_EXPRESSION A%B{C}D
	yprintf("{:%y %w %u}\n", t);
// PASS_REGULAR_EXPRESSION 12 0 7
}
