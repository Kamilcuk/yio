/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>
#include <string.h>
int main() {
#if YYIO_HAS_timespec
	struct timespec t = {
		.tv_sec = 1234567,
		.tv_nsec = 1,
	};
	return yπprintf(TC("{}"), t) == strlen("1234567.000000001") ? 0 : 1;
#endif
}
