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
	yπprintf(TC("{:%Y-%m-%d %H:%M:%S AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA}\n"), t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
}
