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
#if _yIO_HAS_timeval
	struct timeval t = {
		.tv_sec = 1234567,
		.tv_usec = 1,
	};
	return yπprintf(TC("{}"), t) == strlen("1234567.000001") ? 0 : 1;
#endif
}
