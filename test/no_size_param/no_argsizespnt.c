/**
 * @file
 * @date 2020-lip-18
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio/yio/private.h>
int main() {
	(void)sizeof(((yio_printctx_t*)0)->c.argsizespnt);
	return 0;
}
