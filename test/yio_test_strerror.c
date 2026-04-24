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
	err = yio_printf("{} {}", 1);
	yio_printf("{}\n", yio_strerror(err));
	yio_printf("{}\n", yio_strerror(-616278361));
	yio_printf("{}\n", yio_strerror(-1));
	yio_printf("{}\n", yio_strerror(YYIO_ERROR_START));
	yio_printf("{}\n", yio_strerror(YYIO_ERROR_STOP));
	yio_printf("{}\n", yio_strerror(INT_MIN));
	yio_printf("{}\n", yio_strerror(INT_MAX));
}
// PASS_REGULAR_EXPRESSION 1 Too many .*
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
// PASS_REGULAR_EXPRESSION unknown error
