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
	yio_stream(wc, "\n");
	// PASS_REGULAR_EXPRESSION first test

	wchar_t wc2[] = L"second test";
	yio_stream(yio_pwstring(wc2), "\n");
	// PASS_REGULAR_EXPRESSION second test

	const wchar_t cw3[] = L"third test";
	yio_stream(cw3, "\n");
	// PASS_REGULAR_EXPRESSION third test

	const wchar_t cw4[] = L"fourth test";
	yio_stream(yio_pwstring(cw4), "\n");
	// PASS_REGULAR_EXPRESSION fourth test

	wchar_t c = L'H';
	yio_stream(yio_pwchar(c), "\n");
	// PASS_REGULAR_EXPRESSION H
}
