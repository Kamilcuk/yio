/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>

#if \
	defined(YYIO_PRIVATE) || \
	defined(YYIO_YIO_YIO_PRIVATE_H_) || \
	defined(YYIO_YIO_PRIVATE_YIO_RES_H_) || \
	defined(dbgln) || \
	defined(YYIO_ERROR)
#error PRIVATE HEADERS LEAKED TO USERS
#error Regenerate doxygen and see the include graph there
#endif

int main() {
	return 0;
}
