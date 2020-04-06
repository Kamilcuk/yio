/*
 * yio_test_error.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
int main() {
	int err;
	err = yprintf("{} {}", 1);
	printf("%s\n", yio_strerror(err));
	err = yprintf("{a}", 1);
	printf("%s\n", yio_strerror(err));
}
// PASS_REGULAR_EXPRESSION 1 Too many .*
// PASS_REGULAR_EXPRESSION Invalid .*format specifier
