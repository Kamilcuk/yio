/**
 * @file test_bitint.c
 * @brief Test for _BitInt support
 */
#include <yio_test.h>
#include <yio.h>

int main() {
#ifdef __BITINT_MAXWIDTH__
	yio_print("BitInt support detected, maxwidth: {}\n", (int)__BITINT_MAXWIDTH__);

#if __BITINT_MAXWIDTH__ >= 1
	{
		unsigned _BitInt(1) x = 0;
		YIO_TEST("0", "{}", x);
		x = 1;
		YIO_TEST("1", "{}", x);
	}
#endif
#if __BITINT_MAXWIDTH__ >= 2
	{
		_BitInt(2) x = 0;
		YIO_TEST("0", "{}", x);
		x = -1;
		YIO_TEST("-1", "{}", x);
	}
#endif

#if __BITINT_MAXWIDTH__ >= 7
	{
		_BitInt(7) x = 63;
		YIO_TEST("63", "{}", x);
		x = -64;
		YIO_TEST("-64", "{}", x);
	}
#endif

#if __BITINT_MAXWIDTH__ >= 15
	{
		unsigned _BitInt(15) x = 32767;
		YIO_TEST("32767", "{}", x);
		YIO_TEST("0x7fff", "{:#x}", x);
	}
#endif

#if __BITINT_MAXWIDTH__ >= 65
	{
		unsigned _BitInt(65) x = 1;
		x <<= 64;
		// 2^64 = 18446744073709551616
		YIO_TEST("18446744073709551616", "{}", x);
	}
#endif

#if __BITINT_MAXWIDTH__ >= 128
	{
		unsigned _BitInt(128) x = 1;
		x <<= 127;
		// 2^127 = 170141183460469231731687303715884105728
		YIO_TEST("170141183460469231731687303715884105728", "{}", x);
	}
#endif

#else
	yio_print("BitInt not supported by compiler\n");
#endif
	return 0;
}
