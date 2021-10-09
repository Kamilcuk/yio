/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>

m4_divert(-1)
// «m4_stdfix_types»
m4_include(«yio/private/yio_stdfix.h»)
m4_divert(0)

int main() {
m4_applyforeachdefine((m4_stdfix_types), «
	{
		const $2 a = 0.123456789;
		yπprintf(Yc("{:25}: {} {:x} {} {:a}\n"), "$2", (float)a, a, a, a);
	}
»)
}
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
// PASS_REGULAR_EXPRESSION [^:]+: [^ ]+ [0-9a-f]+ 0\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*
