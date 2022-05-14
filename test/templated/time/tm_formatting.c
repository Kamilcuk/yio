/**
 * @file
 * @date 2020-lip-23
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <locale.h>
int main() {
	const struct tm t = {
		.tm_year = 112,
		.tm_mon  = 9,
		.tm_mday = 9,
		.tm_hour = 8,
		.tm_min  = 10,
		.tm_sec  = 20,
	};
	setlocale(LC_ALL, "C");
	YΩIO_TEST("2012-10-09 08:10:20", "{:%Y-%m-%d %H:%M:%S}", t);
	YΩIO_TEST("2", "{:.1%Y-%m-%d %H:%M:%S}", t);
	YΩIO_TEST("---2012-10-09 08:10:20---", "{:-^25%Y-%m-%d %H:%M:%S}", t);
	YΩIO_TEST("2012-10-09 08:10:20------", "{:-<25%Y-%m-%d %H:%M:%S}", t);
	YΩIO_TEST("------2012-10-09 08:10:20", "{:->25%Y-%m-%d %H:%M:%S}", t);
}
