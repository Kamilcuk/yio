#include <yio_test.h>

#ifndef YIO_HAS_WCHAR_H
#error
#endif
#ifndef YYIO_HAS_UNISTRING
#error
#endif

int main() {
	YIO_TEST("a\\\\001\\\\002b", "{!a}", "a\001\002b");
	YYIO_TEST(yprintf("c{!a}d\n", "a\001\002b") == 13);
	YIO_TEST("a\\\\001\\+", "{!a}", "a\x01\x02b");
#if YIO_HAS_WCHAR_H
#if defined(__STDC_ISO_10646__) && WCHAR_MAX == INT32_MAX
	YWIO_TEST("ab\\\\U00000424cd", "a{!a}d", L"b\U00000424c");
#else
	YWIO_TEST("ab\\\\044\\\\004\\\\000\\\\000""cd", "a{!a}d", L"b\U00000424c");
#endif
#endif
#if YYIO_HAS_UNISTRING
	YC16IO_TEST("ab\\\\u0424cd", "a{!a}d", u"b\u0424c");
	YUIO_TEST("ab\\\\U00000424cd", "a{!a}d", U"b\U00000424c");
#endif
}
