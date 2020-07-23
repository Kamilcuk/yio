/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>
int main() {
#ifdef _yIO_HAS_timeval
	struct timeval t = {
		.tv_sec = 1234567,
		.tv_usec = 1,
	};
	return yprintf("{}", t) == strlen("1234567.000001") ? 0 : 1;
#endif
}
