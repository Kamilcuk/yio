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
#ifndef YIO_HAS_WCHAR_H
#error
#endif
#ifndef YIO_HAS_UCHAR_H
#error
#endif
#endif

int main() {
	int a = 1;
#if YIO_HAS_UCHAR_H
	const char16_t nl[] = {u'\n'};
	char16_t s[] = u"hello";
	const char16_t s2[] = u"hello";
	const int ret1 = yπprint(s, " world", a++, "\n");
	const int ret2 = yπprintf(TC("{} {}{}{:.1}"), s2, "world", a++, nl);
	YIO_TESTEXPR(ret1 > 0 && ret2 > 0, "ret1=%d ret2=%d\n", ret1, ret2);
#else
	yπprint("hello world", a++, "\n");
	yπprint("hello world", a++, "\n");
#endif
}
// PASS_REGULAR_EXPRESSION hello world1
// PASS_REGULAR_EXPRESSION hello world2
