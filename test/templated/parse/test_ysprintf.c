/**
 * @file test_ysprintf.c
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yio.h>
#include <stdint.h> // SIZE_MAX
int main() {
	char buf[200];
	int err = 0;

	err = yio_snprintf(buf, sizeof(buf)/sizeof(*buf), "123");
	YIO_TESTEXPR(err > 0, "err=%s", yio_strerror(err));
	YIO_TESTEXPR(strcmp(buf, "123") == 0);

	err = yio_snprintf(buf, SIZE_MAX, "123");
	YIO_TESTEXPR(err > 0);
	YIO_TESTEXPR(strcmp(buf, "123") == 0);

	err = yio_snprintf(buf, 2, "123");
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(strcmp(buf, "1") == 0);

	err = yio_snprintf(buf, 3, "123");
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(strcmp(buf, "12") == 0);

	err = yio_snprintf(buf, 4, "123");
	YIO_TESTEXPR(err > 0);
	YIO_TESTEXPR(strcmp(buf, "123") == 0);

	err = yio_snprintf(buf, 1, "123");
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(strcmp(buf, "") == 0);

	{
		char buf2[11];
		buf2[10] = 'A'; // Canary
		err = yio_snprintf(buf2, 10, "{}{}", "12345", "67890123");
		YIO_TESTEXPR(err < 0);
		YIO_TESTEXPR(buf2[10] == 'A', "Buffer overflow detected! Canary corrupted.");
		YIO_TESTEXPR(strlen(buf2) == 9);
		YIO_TESTEXPR(strcmp(buf2, "123456789") == 0);
	}

	return 0;
}
