/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
int main() {
	int a = 1;
	const char nl[] = {'\n'};
	char s[] = "hello";
	const char s2[] = "hello";
	const int ret1 = yπprint(s, " world", a++, "\n");
	const int ret2 = yπprintf(Yc("{} {}{}{:.1}"), s2, "world", a++, nl);
	_yIO_TEST(ret1 > 0 && ret2 > 0, "ret1=%d ret2=%d\n", ret1, ret2);
}
// PASS_REGULAR_EXPRESSION hello world1
// PASS_REGULAR_EXPRESSION hello world2
