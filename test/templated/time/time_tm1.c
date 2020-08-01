/**
 * @file
 * @date 2020-lip-22
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>
int main() {
	struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	yπprintf(Yc("{:%Y-%m-%d %H:%M:%S}\n"), t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
	yπprintf(Yc("{:%D}\n"), t);
// PASS_REGULAR_EXPRESSION 10/09/12
	yπprintf(Yc("{:%F %T}\n"), t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
	yπprintf(Yc("{:A%%B{{C}}D}\n"), t);
// PASS_REGULAR_EXPRESSION A%B{C}D
	yπprintf(Yc("{:%y %w %u}\n"), t);
// PASS_REGULAR_EXPRESSION 12 0 7
}
