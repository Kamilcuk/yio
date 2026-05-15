/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_stdfix.h"

#ifndef YIO_FIX_CLANG_STDFIX
#error YIO_FIX_CLANG_STDFIX is not defined
#endif

#if YIO_HAS_STDFIX_TYPES

#include "../../private/yio_stdfix.h"
#include "../../private/yio_stdfix_strfrom.h"


{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YIO_STDFIX_$3
int YIO_print_$3(yio_printctx_t *t) {
#if YIO_FIX_CLANG_STDFIX
	/* Clang x86_64 ABI bug. See YIO_FIX_CLANG_STDFIX in configuration.cmake. */
	YIO_STDFIX_$3 v;
	_Static_assert(sizeof(v) <= 8, "stdfix type too large for workaround");
	if (sizeof(v) <= 4) {
		int v_tmp = yio_printctx_va_arg(t, int);
		memcpy(&v, &v_tmp, sizeof(v));
	} else {
		long long v_tmp = yio_printctx_va_arg(t, long long);
		memcpy(&v, &v_tmp, sizeof(v));
	}
#else
	const YIO_STDFIX_$3 v = yio_printctx_va_arg(t, YIO_STDFIX_$3);
#endif
	int err = yio_printctx_init(t);
	if (err) return err;
	struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	YIO_buf res = {0};
	err = YIO_strfrom$1(&res, pf, v);
	if (err) return err;
	const char *const result = YIO_buf_data(&res);
	const size_t length = YIO_buf_len(&res);
	const bool negative = result[0] == '-';
	err = yio_printctx_put_number(t, result + negative, length - negative, !negative);
	YIO_buf_fini(&res);
	return err;
}
#endif
{% endcall %}

#endif // YIO_HAS_STDFIX_TYPES

