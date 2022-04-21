/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#if __NEWLIB__ || __linux__
#ifndef _yIO_HAS_WCHAR_H
#error
#endif
#endif

int main() {
	int a = 1;
#if _yIO_HAS_WCHAR_H
	const wchar_t nl[] = {L'\n'};
	wchar_t s[] = L"hello";
	const wchar_t s2[] = L"hello";
	const int ret1 = yπprint(s, " world", a++, "\n");
	const int ret2 = yπprintf(TC("{} {}{}{:.1}"), s2, "world", a++, nl);
	_yIO_TEST(ret1 > 0 && ret2 > 0, "ret1=%d ret2=%d\n", ret1, ret2);
#else
	yπprint("hello world", a++, "\n");
	yπprint("hello world", a++, "\n");
#endif
}
// PASS_REGULAR_EXPRESSION hello world1
// PASS_REGULAR_EXPRESSION hello world2
