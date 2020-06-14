/**
 * @file test_bits_buf.c
 * @date 2020-06-12
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>
#include <yio/bits/yio_buf.h>

static inline
int _yIO_buf_test_remove_trailing_zeros(const char *in, const char *out) {
	char _buf[20];
	_yIO_buf b = _yIO_buf_init(_buf);
	_yIO_buf_puts(&b, in);
	_yIO_buf_remove_trailing_zeros(&b);
	_yIO_buf_end(&b);
	if (!(strcmp(out, _buf) == 0)) return -__LINE__;
	if (!(b.len == strlen(in))) return -__LINE__;
	return 0;
}

static inline
int _yIO_buf_test(void) {
	int r = 0;
	r = _yIO_buf_test_remove_trailing_zeros("0.00000", "0");
	if (r) return -1000 + r;
	r = _yIO_buf_test_remove_trailing_zeros("0.00001", "0.00001");
	if (r) return -2000 + r;
	r = _yIO_buf_test_remove_trailing_zeros("0.05000", "0.05");
	if (r) return -3000 + r;
	return 0;
}

int main() {
	const int ret = _yIO_buf_test();
	_yIO_TEST(ret == 0, "_yIO_buf_test() = %d", ret);
	return 0;
}
