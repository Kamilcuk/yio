/**
 * @file int_args.c
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yio.h>

int main() {
	char *buf = NULL;
	yio_asprintf(&buf, "{:+0{}}", 3, 10);
	YIO_TESTEXPR(buf != NULL);
	YIO_TESTEXPR(strcmp(buf, "+000000003") == 0);
	yio_asprintf(&buf, "{:+0.{}}", 3, 10);
	YIO_TESTEXPR(buf != NULL);
	YIO_TESTEXPR(strcmp(buf, "+3") == 0);
	yio_asprintf(&buf, "{:+0{}.{}}", 3, 10, 10);
	YIO_TESTEXPR(buf != NULL);
	YIO_TESTEXPR(strcmp(buf, "+000000003") == 0);
	yio_asprintf(&buf, "{:+0.{}}", "12345678901234567890", 10);
	YIO_TESTEXPR(buf != NULL);
	YIO_TESTEXPR(strcmp(buf, "1234567890") == 0);
	free(buf);
	return 0;
}
