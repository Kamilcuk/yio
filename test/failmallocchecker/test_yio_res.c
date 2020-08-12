/**
 * @file
 * @date 2020-sie-05
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio/private/yio_res.h>
#include <yio_test.h>
#include <failmallocchecker.h>
#include <assert.h>

int main() {
	char _buf[200];
	_yIO_res _res_buf;
	_yIO_res * const o = &_res_buf;
	int err = 0;

	FMC_FOR(ok) {
		char *buf = NULL; size_t len = 0;
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add to the container";
		err = _yIO_res_puts(o, s1);
		if (ok) {
			_yIO_TEST(err == 0);
			const size_t lenr = _yIO_res_end(o, &buf, &len);
			_yIO_TEST(lenr == len);
			_yIO_TEST(buf != NULL);
			_yIO_TEST(len = strlen(s1));
			_yIO_TEST(memcmp(buf, s1, len) == 0);
			_yIO_res_free_result(NULL, buf);
		} else {
			_yIO_TEST(err != 0);
		}
	}

	FMC_FOR(ok) {
		char *buf = _buf; size_t len = sizeof(_buf);
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add but lower then _buf";
		err = _yIO_res_puts(o, s1);
		_yIO_TEST(err == 0);
		const size_t lenr = _yIO_res_end(o, &buf, &len);
		_yIO_TEST(lenr == len);
		_yIO_TEST(memcmp(buf, s1, len) == 0);
		_yIO_TEST(buf == _buf);
		(void)ok;
		_yIO_res_free_result(_buf, buf);
	}

	FMC_FOR(ok) {
		char *buf = _buf; size_t len = 10;
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add but longer then _buf";
		err = _yIO_res_puts(o, s1);
		if (ok) {
			_yIO_TEST(err == 0);
			const size_t lenr = _yIO_res_end(o, &buf, &len);
			_yIO_TEST(lenr == len);
			_yIO_TEST(memcmp(buf, s1, len) == 0);
			_yIO_TEST(buf != _buf);
			_yIO_res_free_result(_buf, buf);
		} else {
			_yIO_TEST(err != 0);
		}
	}

	FMC_FOR(ok) {
		char *buf = _buf; size_t len = sizeof(_buf);
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add but longer then _buf";
		assert(sizeof(_buf) > 3 * sizeof(s1));
		err = _yIO_res_puts(o, s1);
		_yIO_TEST(err == 0);
		err = _yIO_res_puts(o, s1);
		_yIO_TEST(err == 0);
		err = _yIO_res_puts(o, s1);
		_yIO_TEST(err == 0);
		const size_t lenr = _yIO_res_end(o, &buf, &len);
		_yIO_TEST(lenr == len);
		_yIO_TEST(memcmp(buf, s1, len/3) == 0);
		_yIO_TEST(memcmp(buf+len/3, s1, len/3) == 0);
		_yIO_TEST(memcmp(buf+2*len/3, s1, len/3) == 0);
		_yIO_TEST(buf == _buf);
		(void)ok;
		_yIO_res_free_result(_buf, buf);
	}

	FMC_FOR(ok) {
		char *buf = _buf; size_t len = sizeof(_buf);
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add";
		for (size_t i = 0; i < strlen(s1); ++i) {
			err = _yIO_res_putc(o, s1[i]);
			_yIO_TEST(err == 0);
		}
		const size_t lenr = _yIO_res_end(o, &buf, &len);
		_yIO_TEST(lenr == len);
		_yIO_TEST(memcmp(buf, s1, len) == 0);
		_yIO_TEST(buf == _buf);
		(void)ok;
		_yIO_res_free_result(_buf, buf);
	}

	FMC_FOR(ok) {
		char *buf = _buf; size_t len = 15;
		_yIO_res_init(o, &buf, &len);
		const char s1[] = "a string to add but longer then _buf";
		err = 0;
		for (size_t i = 0; i < strlen(s1); ++i) {
			err = _yIO_res_putc(o, s1[i]);
			if (err) break;
		}
		if (ok) {
			const size_t lenr = _yIO_res_end(o, &buf, &len);
			_yIO_TEST(lenr == len);
			_yIO_TEST(memcmp(buf, s1, len) == 0);
			_yIO_TEST(buf != _buf);
			_yIO_res_free_result(_buf, buf);
		} else {
			_yIO_TEST(err != 0);
		}
	}
}
