/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>
int main() {
	struct tm t = {
		.tm_year = 0,
		.tm_mon  = 0,
		.tm_mday = 1,
		.tm_hour = 20,
		.tm_min  = 0,
		.tm_sec  = 0,
	};
	int err = 0;
	err |= yπprintf(TC("@{:%Y-%m-%d %H:%M:%S}@\n"), t);
// PASS_REGULAR_EXPRESSION @1900-01-01 20:00:00@
	err |= yπprintf(TC("@{:%D}@\n"), t);
// PASS_REGULAR_EXPRESSION @01/01/00@
	const struct tm t2 = t;
	err |= yπprintf(TC("@{:%F %T}@\n"), t2);
// PASS_REGULAR_EXPRESSION @1900-01-01 20:00:00@
	err |= yπprintf(TC("@{:%y %w %u}@\n"), t2);
// PASS_REGULAR_EXPRESSION @00 0 7@
	return err < 0;
}
