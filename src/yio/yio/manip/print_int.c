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
const TCHAR *_yΩIO_printint_to_fmt(TCHAR type) {
	if (type == TC('x')) {
		return TC("0123456789abcdef");
	}
	return TC("0123456789ABCDEF");
}

{% call(V) j_FOREACHAPPLY([
	["uschar", "unsigned char"],
	["schar", "signed char"],
	["ushort", "unsigned short"],
	["short", "short"],
	["uint", "unsigned int"],
	["int", "int"],
	["ulong", "unsigned long"],
	["long", "long"],
	["ullong", "unsigned long long"],
	["llong", "long long"],
	["u__int128", "unsigned __int128", "_yIO_HAS_INT128"],
	["__int128", "__int128", "_yIO_HAS_INT128"],
	]) %}

{% if V.2 is defined %}
#line
#ifndef $3
#error
#endif
#if $3
{% endif %}
#line

{% if j_match(V.1, "unsigned") %}
#line

static inline
int _yΩIO_print_$1_inradix(yπio_printctx_t *t, $2 arg, bool is_negative,
		TCHAR type, $2 radix, TCHAR *res, size_t ressize) {
	const TCHAR *fmt = _yΩIO_printint_to_fmt(type);
	TCHAR *const resend = res + (ressize / sizeof(*res));
	TCHAR *num = resend;
	do {
		(--num)[0] = fmt[arg % radix];
	} while (arg /= radix);
	assert(res <= num);
	const size_t length = resend - num;
	return yπio_printctx_putπ_number(t, num, length, !is_negative);
}

static inline
int _yΩIO_print_$1_in(yπio_printctx_t *t, $2 arg, bool is_negative) {
	const TCHAR type = yπio_printctx_get_fmt(t)->type;
	switch (type) {
		case TC('\0'):
		case TC('d'):
			{
				TCHAR buf[_yIO_LOG10_POW2(sizeof($2) * CHAR_BIT)];
				return _yΩIO_print_$1_inradix(t, arg, is_negative, type, 10, buf, sizeof(buf));
			}
		case TC('o'):
		case TC('O'):
			{
				TCHAR buf[(sizeof($2) * CHAR_BIT) / 3 + !!((sizeof($2) * CHAR_BIT) % 3)];
				return _yΩIO_print_$1_inradix(t, arg, is_negative, type, 8, buf, sizeof(buf));
			}
		case TC('x'):
		case TC('X'):
			{
				TCHAR buf[(sizeof($2) * CHAR_BIT) / 4 + !!((sizeof($2) * CHAR_BIT) % 4)];
				return _yΩIO_print_$1_inradix(t, arg, is_negative, type, 16, buf, sizeof(buf));
			}
		case TC('b'):
		case TC('B'):
			{
				TCHAR buf[sizeof($2) * CHAR_BIT];
				return _yΩIO_print_$1_inradix(t, arg, is_negative, type, 2, buf, sizeof(buf));
			}
		default:
			return YIO_ERROR_UNKNOWN_FMT;
	}
}

int _yΩIO_print_$1(yπio_printctx_t *t) {
	const $2 arg = yπio_printctx_va_arg_promote(t, $2);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return _yΩIO_print_$1_in(t, arg, false);
}

{% else %}
#line

int _yΩIO_print_$1(yπio_printctx_t *t) {
	const $2 arg = yπio_printctx_va_arg_promote(t, $2);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	const bool is_negative = arg < 0;
{% if j_match(V.1, "signed char") %} #line
	typedef unsigned char unsignedtype;
{% else %} #line
	typedef unsigned $2 unsignedtype;
{% endif %} #line
	const unsignedtype uarg = is_negative ? -((unsignedtype)arg) : (unsignedtype)arg;
	return _yΩIO_print_u$1_in(t, uarg, is_negative);
}

{% endif %}

{% if V.2 is defined %}
#endif // $3
{% endif %}
{% endcall %}
