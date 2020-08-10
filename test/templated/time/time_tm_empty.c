/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>
#include <locale.h>
int main() {
	struct tm t = {
		.tm_year = 0,
		.tm_mon  = 0,
		.tm_mday = 1,
		.tm_hour = 20,
		.tm_min  = 0,
		.tm_sec  = 0,
	};
	setlocale(LC_ALL, "C");
	yπprintf(Yc("{}\n"), t);
	yπprintf(Yc("{:}\n"), t);
}
// PASS_REGULAR_EXPRESSION Sun Jan  1 20:00:00 1900
// PASS_REGULAR_EXPRESSION Sun Jan  1 20:00:00 1900
