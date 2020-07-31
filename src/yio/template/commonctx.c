/**
 * @file
 * @date 2020-05-10
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <limits.h>
#include <ctype.h>
#include <assert.h>

int _yΩIO_digit_to_number(Ychar d) {
#if _yIO_TYPE_YIO
	return d - '0';
#elif _yIO_TYPE_YWIO || _yIO_TYPE_YUIO
	const Ychar table[] = Yc("0123456789");
	return Ystrchr(table, d) - table;
#else
#error
#endif
}

int _yΩIO_commonctx_strtoi_noerr(const Ychar **ptr) {
	int num = 0;
	do {
		assert(num < INT_MAX / 10);
		num *= 10;
		const int c = _yΩIO_digit_to_number((*ptr)[0]);
		assert(num < INT_MAX - c);
		num += c;
		++(*ptr);
	} while (Yisdigit((*ptr)[0]));
	return num;
}

int _yΩIO_commonctx_stdintparam(int (*get_va_arg_int)(void *arg), void *arg,
		const Ychar *ptr, const Ychar **endptr, int *res) {
	int num = -1;
	int ret = 0;
	if (ptr[0] == Yc('{')) {
		ptr++;
		if (Yisdigit(ptr[0])) {
			return -ENOSYS;
		}
		if (ptr++[0] != Yc('}')) {
			ret = YIO_ERROR_FMT_INVALID;
			goto EXIT;
		}
		num = get_va_arg_int(arg);
	}
	if (Yisdigit(ptr[0])) {
		num = _yΩIO_commonctx_strtoi_noerr(&ptr);
	}
	EXIT:
	*endptr = ptr;
	*res = num;
	return ret;
}

