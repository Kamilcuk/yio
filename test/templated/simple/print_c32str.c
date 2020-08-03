/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#if __linux__
#ifndef _yIO_HAS_WCHAR_H
#error
#endif
#ifndef _yIO_HAS_UCHAR_H
#error
#endif
#endif

#if __NEWLIB__
#ifndef _yIO_HAS_WCHAR_H
#error
#endif
#endif

int main() {
	int a = 1;
#ifdef _yIO_HAS_UCHAR_H
	const char32_t nl[] = {U'\n'};
	char32_t s[] = U"hello";
	const char32_t s2[] = U"hello";
	const int ret1 = yπprint(s, " world", a++, "\n");
	const int ret2 = yπprintf(Yc("{} {}{}{:.1}"), s2, "world", a++, nl);
	_yIO_TEST(ret1 > 0 && ret2 > 0, "ret1=%d ret2=%d\n", ret1, ret2);
#else
	yπprint("hello world", a++, "\n");
	yπprint("hello world", a++, "\n");
#endif
}
// PASS_REGULAR_EXPRESSION hello world1
// PASS_REGULAR_EXPRESSION hello world2
