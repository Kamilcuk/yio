#include <yio_test.h>

#ifndef YIO_HAS_WCHAR_H
#error
#endif
#ifndef YYIO_HAS_UNISTRING
#error
#endif

int main() {
	YIO_TEST(( .eq="a\\1\\2b", .ret=6 ), "{!a}", "a\001\2b");
	YIO_TEST("a\\1+", "{!a}", "a\x01\x02b");
	YIO_TEST("a\\0011", "{!a}", "a\0011");
	YIO_TEST("?\\?=", "{!a}", "?\?=");
#if YIO_HAS_WCHAR_H
#if defined(__STDC_ISO_10646__) && WCHAR_MAX == INT32_MAX
#else
#endif
#endif
#if YYIO_HAS_UNISTRING
	YC16IO_TEST("ab\\u0424cd", "a{!a}d", u"b\u0424c");
	YUIO_TEST("ab\\U00000424cd", "a{!a}d", U"b\U00000424c");
#endif
}
