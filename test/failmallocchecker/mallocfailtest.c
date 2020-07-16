/**
 * @file
 * @date 2020-lip-15
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <failmallocchecker.h>
#include <yio.h>
#include <yio_test.h>

void test1_in(int ok) {
	int ret;
	ret = yprintf("{} {} {} {}", 1.234f, 2.345, 1.234f, 2.345);
	int ret2 = yprintf("\n");
	_yIO_TEST(ret2 > 0);
	fflush(stdout);
	if (ok) {
		_yIO_TEST(ret > 0);
	} else {
		_yIO_TEST(ret == YIO_ERROR_ENOMEM, "ret=%d", ret);
	}
}

int main() {
	test1_in(1);

	FMC_FOR(ok) {
		test1_in(ok);
	}

	FMC_FOR(ok) {
		char *str;
		int ret = yaprintf(&str, "{} {} {}", 1, 1.234f, 2.345);
		fflush(stdout);
		if (ok) {
			_yIO_TEST(ret > 0);
			_yIO_TEST(str != NULL);
		} else {
			_yIO_TEST(ret == YIO_ERROR_ENOMEM, "ret=%d", ret);
			_yIO_TEST(str == NULL, "str=%p", str);
		}
		if (str) {
			yprintf("{}\n", str);
			free(str);
		}
	}

	FMC_FOR(ok) {
		char *str = yformatf("{} {} {} {}", "bla", 1, 1.234f, 2.345);
		if (ok) {
			_yIO_TEST(str != NULL);
			free(str);
		} else {
			_yIO_TEST(str == NULL);
		}
	}
}
