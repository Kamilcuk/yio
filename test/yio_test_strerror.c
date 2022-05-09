/*
 * yio_test_error.c
 *
 *  Created on: 5 kwi 2020
 *      Author: kamil
 */
#include <yio.h>
#include <limits.h>
int main() {
	int err;
	err = yprintf("{} {}", 1);
	yprintf("{}\n", yio_strerror(err));
	yprintf("{}\n", yio_strerror(-616278361));
	yprintf("{}\n", yio_strerror(-1));
	yprintf("{}\n", yio_strerror(_yIO_ERROR_START));
	yprintf("{}\n", yio_strerror(_yIO_ERROR_STOP));
	yprintf("{}\n", yio_strerror(INT_MIN));
	yprintf("{}\n", yio_strerror(INT_MAX));
}
// PASS_REGULAR_EXPRESSION 1 Too many .*
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
