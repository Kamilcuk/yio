/*
 * yio_test_100.c
 *
 *  Created on: 30 mar 2020
 *      Author: Kamil
 */
#include <yio.h>
#include <string.h>

#ifndef __GNUC__
#pragma GCC optimize("O")
int main() {
	const char msg[] = "hello world";
	const size_t a = _yIO_STRLEN(msg);
	if (__builtin_constant_p(a) && a == strlen(msg)) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
#else
int main() {
	return EXIT_SUCCESS;
}
#endif
