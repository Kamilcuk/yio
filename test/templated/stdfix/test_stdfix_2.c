/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yπio.h>

#ifdef fract
#error leaked stdfix.h to public headers
#endif

m4_divert(-1)
// `m4_stdfix_types~
m4_include(`yio/private/yio_stdfix.h~)
m4_divert(0)m4_dnl;

int main() {
	{
		const short _Fract a = 0.123456789;
		yπprintf(Yc("{:x} {} {:a}\n"), a, a, a);
	}
	{
		const _Fract a = 0.123456789;
		yπprintf(Yc("{:x} {} {:a}\n"), a, a, a);
	}
	{
		const long _Fract a = 0.123456789;
		yπprintf(Yc("{:x} {} {:a}\n"), a, a, a);
	}
}
// PASS_REGULAR_EXPRESSION f 0.12[^ ]* [^ ]*
// PASS_REGULAR_EXPRESSION fcd 0.123[^ ]* [^ ]*
// PASS_REGULAR_EXPRESSION fcd6e9b 0.123457 [^ ]*