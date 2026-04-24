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
const char *YYIO_printint_to_fmt(char type) {
	if (type == 'x') {
		return "0123456789abcdef";
	}
	return "0123456789ABCDEF";
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
	["u__int128", "unsigned __int128", "YYIO_HAS_INT128"],
	["__int128", "__int128", "YYIO_HAS_INT128"],
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
int YYIO_print_$1_inradix(yio_printctx_t *t, $2 arg, bool is_negative,
		char type, $2 radix, char *res, size_t ressize) {
	const char *fmt = YYIO_printint_to_fmt(type);
	char *const resend = res + (ressize / sizeof(*res));
	char *num = resend;
	do {
		(--num)[0] = fmt[arg % radix];
	} while (arg /= radix);
	assert(res <= num);
	const size_t length = resend - num;
	return yio_printctx_put_number(t, num, length, !is_negative);
}

int YYIO_print_$1_in(yio_printctx_t *t, $2 arg, bool is_negative) {
	const char type = yio_printctx_get_fmt(t)->type;
	switch (type) {
		case '\0':
		case 'd':
			{
				char buf[YYIO_LOG10_POW2(sizeof($2) * CHAR_BIT)];
				return YYIO_print_$1_inradix(t, arg, is_negative, type, 10, buf, sizeof(buf));
			}
		case 'o':
		case 'O':
			{
				char buf[(sizeof($2) * CHAR_BIT) / 3 + (((sizeof($2) * CHAR_BIT) % 3) != 0)];
				return YYIO_print_$1_inradix(t, arg, is_negative, type, 8, buf, sizeof(buf));
			}
		case 'x':
		case 'X':
			{
				char buf[(sizeof($2) * CHAR_BIT) / 4 + (((sizeof($2) * CHAR_BIT) % 4) != 0)];
				return YYIO_print_$1_inradix(t, arg, is_negative, type, 16, buf, sizeof(buf));
			}
		case 'b':
		case 'B':
			{
				char buf[sizeof($2) * CHAR_BIT];
				return YYIO_print_$1_inradix(t, arg, is_negative, type, 2, buf, sizeof(buf));
			}
		default:
			return YIO_ERROR_UNKNOWN_FMT;
	}
}

int YYIO_print_$1(yio_printctx_t *t) {
	const $2 arg = yio_printctx_va_arg_promote(t, $2);
	const int err = yio_printctx_init(t);
	if (err) return err;
	return YYIO_print_$1_in(t, arg, false);
}

{% else %}
#line

int YYIO_print_$1(yio_printctx_t *t) {
	const $2 arg = yio_printctx_va_arg_promote(t, $2);
	const int err = yio_printctx_init(t);
	if (err) return err;
	const bool is_negative = arg < 0;
{% if j_match(V.1, "signed char") %} #line
	typedef unsigned char unsignedtype;
{% else %} #line
	typedef unsigned $2 unsignedtype;
{% endif %} #line
	const unsignedtype uarg = is_negative ? -((unsignedtype)arg) : (unsignedtype)arg;
	return YYIO_print_u$1_in(t, uarg, is_negative);
}

{% endif %}

{% if V.2 is defined %}
#endif // $3
{% endif %}
{% endcall %}
