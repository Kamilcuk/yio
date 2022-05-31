/**
 * @file test_ysprintf.c
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yπio.h>
#include <stdint.h> // SIZE_MAX
int main() {
	TCHAR buf[200];
	int err = 0;

	err = yπsprintf(buf, sizeof(buf)/sizeof(*buf), TC("123"));
	YIO_TESTEXPR(err > 0, "err=%s", yio_strerror(err));
	YIO_TESTEXPR(TSTRCMP(buf, TC("123")) == 0);

	err = yπsprintf(buf, SIZE_MAX, TC("123"));
	YIO_TESTEXPR(err > 0);
	YIO_TESTEXPR(TSTRCMP(buf, TC("123")) == 0);

	err = yπsprintf(buf, 2, TC("123"));
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(TSTRCMP(buf, TC("1")) == 0);

	err = yπsprintf(buf, 3, TC("123"));
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(TSTRCMP(buf, TC("12")) == 0);

	err = yπsprintf(buf, 4, TC("123"));
	YIO_TESTEXPR(err > 0);
	YIO_TESTEXPR(TSTRCMP(buf, TC("123")) == 0);

	err = yπsprintf(buf, 1, TC("123"));
	YIO_TESTEXPR(err < 0);
	YIO_TESTEXPR(TSTRCMP(buf, TC("")) == 0);

	return 0;
}
