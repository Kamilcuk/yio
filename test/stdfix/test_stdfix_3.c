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

#ifndef YIO_HAS_STDFIX_TYPES
#error YIO_HAS_STDFIX_TYPES is not defined
#endif

#ifdef fract
#error leaked stdfix.h to public headers
#endif

static int hex_dot_hex_to_int(char *str) {
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

#include <stdio.h>
int main() {
	#if !YIO_HAS_STDFIX_TYPES
	return EXIT_SKIP;
	#else
	char buf[128];
	int ret;
	{
		const _Fract a = 0.5r;
		ret = yio_snprintf(buf, sizeof(buf), "{}", a);
		YIO_TESTEXPR(ret == 8, "Got: %d", ret);
		YIO_TESTEXPR(strcmp(buf, "0.500000") == 0, "Got: %s", buf);
	}
	{
		const _Fract a = 0.25r;
		ret = yio_snprintf(buf, sizeof(buf), "{}", a);
		YIO_TESTEXPR(ret == 8, "Got: %d", ret);
		YIO_TESTEXPR(strcmp(buf, "0.250000") == 0, "Got: %s", buf);
	}
	{
		const _Accum a = 0.5k;
		ret = yio_snprintf(buf, sizeof(buf), "{}", a);
		YIO_TESTEXPR(ret == 8, "Got: %d", ret);
		YIO_TESTEXPR(strcmp(buf, "0.500000") == 0, "Got: %s", buf);
	}
	{
		const short _Fract x = 0.123456789;
		ret = yio_snprintf(buf, sizeof(buf), "{:.1a}", x);
		YIO_TESTEXPR(ret > 0, "Got: %d", ret);
		unsigned a, b; int c;
		YIO_TESTEXPR(sscanf(buf, "0x%x.%xp%x", &a, &b, &c) == 3, "%s", buf);
		int v = b << 4 | a;
		yio_printf("{:x} {:x}\n", v, x);
	}
	{
		const _Fract a = 0.123456789;
		yio_printf("{:x} {} {:a}\n", a, a, a);
	}
	{
		const long _Fract a = 0.123456789;
		yio_printf("{:x} {} {:a}\n", a, a, a);
	}
#endif
}
