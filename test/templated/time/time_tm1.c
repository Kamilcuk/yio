/**
 * @file
 * @date 2020-lip-22
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yπio.h>
#include <assert.h>
int main() {
	struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	int err = 0;
	err |= yπprintf(TC("{:%Y-%m-%d %H:%M:%S}\n"), t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
	err |= yπprintf(TC("{:%D}\n"), t);
// PASS_REGULAR_EXPRESSION 10/09/12
	err |= yπprintf(TC("{:%F %T}\n"), t);
// PASS_REGULAR_EXPRESSION 2012-10-09 08:10:20
// FUN FACT: this file is templated with jinja, so {_{ are parsed by jinja.
	err |= yπprintf(TC("{:A%%B{""{C}""}""}D\n"), t);
// PASS_REGULAR_EXPRESSION A%B{[{]C}D
	err |= yπprintf(TC("{:%y %w %u}\n"), t);
// PASS_REGULAR_EXPRESSION 12 0 7
	YYIO_TEST(err > 0);
}
