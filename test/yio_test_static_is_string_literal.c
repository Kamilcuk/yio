/*
 * yio_test_static_is_string_literal.c
 *
 *  Created on: 30 mar 2020
 *      Author: Kamil
 */
#include <yio_test_private.h>
int main() {
	_yIO_TEST(_yIO_IS_STRING_LITERAL("abc"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u8"abc"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u"abc"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(U"abc"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(L"abc"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(""));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u8""));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u""));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(U""));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(L""));
	_yIO_TEST(_yIO_IS_STRING_LITERAL("\n"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u8"\n"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u"\n"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(U"\n"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(L"\n"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL("a"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u8"a"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(u"a"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(U"a"));
	_yIO_TEST(_yIO_IS_STRING_LITERAL(L"a"));
	const char *msg = NULL, a = 0, b = 0;
	(void)msg;
	(void)a;
	(void)b;
	_yIO_TEST(!_yIO_IS_STRING_LITERAL(msg));
	_yIO_TEST(!_yIO_IS_STRING_LITERAL(a));
	_yIO_TEST(!_yIO_IS_STRING_LITERAL(#));
	_yIO_TEST(!_yIO_IS_STRING_LITERAL(b + a));
	_yIO_TEST(!_yIO_IS_STRING_LITERAL("a"[0]));
	_yIO_TEST(!_yIO_IS_STRING_LITERAL(*""));

	_yIO_TEST(_yIO_IS_CHARACTER_STRING_LITERAL("a"));
	_yIO_TEST(_yIO_IS_CHARACTER_STRING_LITERAL("abc"));
	_yIO_TEST(_yIO_IS_CHARACTER_STRING_LITERAL("abc" "def"));
	_yIO_TEST(!_yIO_IS_CHARACTER_STRING_LITERAL(L"abc" "def"));
	_yIO_TEST(!_yIO_IS_CHARACTER_STRING_LITERAL(L"abc" "def"));
	_yIO_TEST(!_yIO_IS_CHARACTER_STRING_LITERAL(nfsdjnfd nasj));

	_yIO_TEST(!_yIO_IS_WIDE_STRING_LITERAL("a"));
	_yIO_TEST(!_yIO_IS_WIDE_STRING_LITERAL("abc"));
	_yIO_TEST(_yIO_IS_WIDE_STRING_LITERAL(L"abc" "def"));
	_yIO_TEST(!_yIO_IS_WIDE_STRING_LITERAL(u8"abc" "def"));
	_yIO_TEST(!_yIO_IS_WIDE_STRING_LITERAL(U"abc" "def"));
	_yIO_TEST(!_yIO_IS_WIDE_STRING_LITERAL(fdsnjfsdan));

	return 0;
}
