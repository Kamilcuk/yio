/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>
#include <yio_test.h>

#ifdef fract
#error leaked stdfix.h to public headers
#endif

m4_divert(-1)
// `m4_stdfix_types~
m4_include(`yio/private/yio_stdfix.h~)
m4_divert(0)m4_dnl;

int hex_dot_hex_to_int(char *str) {
	int r = 0;
	while (*str != 0) {
		char c = str[0];
		r |= strchr("0123456789", c) ? c - '0' : (10 + c - 'f');
		r <<= 4;
		if (*str == '.') {
			str++;
		}
	}
	return r;
}

int main() {
	{
		const short _Fract x = 0.123456789;
		char *tmp = yformatf("{:.1a}\n", x);
		unsigned a, b; int c;
		_yIO_TEST(sscanf(tmp, "0x%x.%xp%x", &a, &b, &c) == 3, "%s", tmp);
		free(tmp);
		int v = b << 4 | a;
		yprintf("{:x} {:x}\n", v, x);

	}
	{
		const _Fract a = 0.123456789;
		yprintf("{:x} {} {:a}\n", a, a, a);
	}
	{
		const long _Fract a = 0.123456789;
		yprintf("{:x} {} {:a}\n", a, a, a);
	}
}
