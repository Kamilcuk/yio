/**
 * @file
 * @date 2020-lip-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

#define TEST(FORMAT, RESULT) do{ \
		char dest[250]; \
		int ret = _yIO_print_time_in_extract_format_add_space(dest, FORMAT, NULL); \
		int ret2 = _yIO_print_time_in_extract_format_add_space(NULL, FORMAT, NULL); \
		_yIO_TEST(ret == ret2, "ret=%d ret2=%d\n", ret, ret2); \
		if (RESULT == NULL) { \
			_yIO_TEST(ret < 0, "result is NULL, but ret=%d - FORMAT='%s'", ret, FORMAT); \
		} else { \
			_yIO_TEST((size_t)ret < sizeof(dest), \
					"ret=%d sizeof(dest)=%d FORMAT='%s'", (int)ret, (int)sizeof(dest), FORMAT); \
			const char *resultnowarn = RESULT == NULL ? "" : RESULT; \
			_yIO_TEST(strcmp(dest, resultnowarn) == 0, \
					"FORMAT='%s' dest='%s' result='%s'	", FORMAT, dest, resultnowarn); \
		} \
	} while (0)

int main() {
	TEST("", NULL);
	TEST(" abc def ", NULL);
	TEST(" { }", NULL);
	TEST(" }", "  ");
	TEST(" } }", "  ");
	TEST("}", "%c ");
	TEST("abc}", "abc ");
	TEST(" {{ }} ", NULL);
	TEST(" {{ }} }", " { }  ");
}
