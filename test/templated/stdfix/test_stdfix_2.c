/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#ifdef fract
#error leaked stdfix.h to public headers
#endif

int main() {
	{
		const short _Fract a = 0.123456789;
		YIO_TEST((.rgx="f 0.1[12][^ ]* [^ ]*"), "{:x} {} {:a}", a, a, a);
	}
	{
		const _Fract a = 0.123456789;
		YIO_TEST((.rgx="fcd 0.123[^ ]* [^ ]*"), "{:x} {} {:a}", a, a, a);
	}
	{
		const long _Fract a = 0.123456789;
		YIO_TEST((.rgx="fcd6e9b 0.123457 [^ ]*"), "{:x} {} {:a}", a, a, a);
	}
}
