/**
 * @file wchars.c
 * @date 2020-06-15
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio.h>

int main() {
	wchar_t wc[] = L"first test";
	yprint(wc, "\n");
	// PASS_REGULAR_EXPRESSION first test

	wchar_t wc2[] = L"second test";
	yprint(ypwstring(wc2), "\n");
	// PASS_REGULAR_EXPRESSION second test

	const wchar_t cw3[] = L"third test";
	yprint(cw3, "\n");
	// PASS_REGULAR_EXPRESSION third test

	const wchar_t cw4[] = L"fourth test";
	yprint(ypwstring(cw4), "\n");
	// PASS_REGULAR_EXPRESSION fourth test

	wchar_t c = L'H';
	yprint(ypwchar(c), "\n");
	// PASS_REGULAR_EXPRESSION H
}
