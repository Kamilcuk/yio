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

#ifndef YYIO_FIX_CLANG_STDFIX
#error YYIO_FIX_CLANG_STDFIX is not defined
#endif

#if YYIO_HAS_STDFIX_TYPES

#include "../../private/yio_stdfix.h"
#include "../../private/yio_stdfix_strfrom.h"


{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YYIO_STDFIX_$3
int YYIO_print_$3(yio_printctx_t *t) {
#if YYIO_FIX_CLANG_STDFIX
	/* Clang x86_64 ABI bug. See YYIO_FIX_CLANG_STDFIX in configuration.cmake. */
	YYIO_STDFIX_$3 v;
	_Static_assert(sizeof(v) <= 8, "stdfix type too large for workaround");
	if (sizeof(v) <= 4) {
		int v_tmp = yio_printctx_va_arg(t, int);
		memcpy(&v, &v_tmp, sizeof(v));
	} else {
		long long v_tmp = yio_printctx_va_arg(t, long long);
		memcpy(&v, &v_tmp, sizeof(v));
	}
#else
	const YYIO_STDFIX_$3 v = yio_printctx_va_arg(t, YYIO_STDFIX_$3);
#endif
	int err = yio_printctx_init(t);
	if (err) return err;
	struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	YYIO_string res = {0};
	err = YYIO_astrfrom$1(&res, pf, v);
	if (err) return err;
	const char *const result = YYIO_string_data(&res);
	const size_t length = YYIO_string_len(&res);
	const bool negative = result[0] == '-';
	err = yio_printctx_put_number(t, result + negative, length - negative, !negative);
	YYIO_string_free(&res);
	return err;
}
#endif
{% endcall %}

#endif // YYIO_HAS_STDFIX_TYPES

