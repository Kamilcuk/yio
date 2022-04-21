/**
#line
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __INTEL_COMPILER
#pragma warning disable 3179
#endif

/* ------------------------------------------------------------------------------- */

static inline
int _yΩIO_printint_to_radix(TCHAR type) {
	switch (type) {
	case 0:
	case TC('d'):
		return 10;
	case TC('x'):
	case TC('X'):
		return 16;
	case TC('o'):
		return 8;
	case TC('b'):
		return 2;
	}
	return -1;
}


static inline
const TCHAR *_yΩIO_printint_to_fmt(TCHAR type) {
	if (type == TC('x')) {
		return TC("0123456789abcdef");
	}
	return TC("0123456789ABCDEF");
}

{% call(V) j_FOREACHAPPLY([
	["short", "short"],
	["ushort", "unsigned short"],
	["int", "int"],
	["uint", "unsigned int"],
	["long", "long"],
	["ulong", "unsigned long"],
	["llong", "long long"],
	["ullong", "unsigned long long"],
	["__int128", "__int128", "_yIO_HAS_INT128"],
	["u__int128", "unsigned __int128", "_yIO_HAS_INT128"],
	]) %}

{% if V.2 is defined %}
#line
#ifndef $3
#error
#endif
#if $3
{% endif %}
#line

int _yΩIO_print_$1(yπio_printctx_t *t) {
	$2 arg = yπio_printctx_va_arg_num(t, $2);
	int err = yπio_printctx_init(t);
	if (err) return err;
	const TCHAR type = yπio_printctx_get_fmt(t)->type;
	const $2 radix = _yΩIO_printint_to_radix(type);
	if (radix == ($2)-1) {
		return YIO_ERROR_UNKNOWN_FMT;
	}
	const TCHAR *format = _yΩIO_printint_to_fmt(type);
	const bool negative =
	{# If $2 is unsigned, it can't be negative So remove the check, so that -Wtype-limits doesn't throw #}
	{% if j_match(V.1, "unsigned") %}
#line
			false;
	{% else %}
#line
			arg < 0;
	{% endif %}
#line

	TCHAR res[sizeof($2) * CHAR_BIT + 1] = {0};
	TCHAR *const res_end = res + sizeof(res)/sizeof(*res);
	TCHAR *num = res_end;
	const $2 tmp = arg % radix;
	(--num)[0] = format[negative ? -tmp : tmp];
	if (arg /= radix) {
		if (negative) {
			arg = -arg;
		}
		do {
			(--num)[0] = format[arg % radix];
		} while (arg /= radix);
	}
	const size_t length = res_end - num;
	return yπio_printctx_putπ_number(t, num, length, !negative);
}

{% if V.2 is defined %}
#endif // $3
{% endif %}
{% endcall %}
