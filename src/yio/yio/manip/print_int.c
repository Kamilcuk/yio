/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include "print_int.h"
#include "print_int_private.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __INTEL_COMPILER
#pragma warning disable 3179
#endif

#ifndef YYIO_HAS_INT128
#error YYIO_HAS_INT128 is not defined
#endif

/* ------------------------------------------------------------------------------- */

{# List of types that we want to define printers for #}
{% set printers_types = [
	["uint", "unsigned int"],
	["int", "int"],
	["ulong", "unsigned long"],
	["long", "long"],
	["ullong", "unsigned long long", "YYIO_HAS_LLONG"],
	["llong", "long long", "YYIO_HAS_LLONG"],
	["uint128", "unsigned __int128", "YYIO_HAS_INT128"],
	["int128", "__int128", "YYIO_HAS_INT128"],
	["ubitint128", "unsigned _BitInt(128)", "!YYIO_HAS_INT128 && YYIO_BITINT_MAXWIDTH >= 128"],
] %}

{# All types that will have custom printers related to BitInt types above 128. #}
{# 128 a bit special - we use __int128 if available, already added above. #}
{% set bitint_printers = [] %}
{% if 1 %}
	{% for i in j_one_to_n(j_BITINT_MAXWIDTH) %}
		{% if i > 128 and j_is_power_of_two(i) %}
				{% do bitint_printers.append(i) %}
		{% endif %}
	{% endfor %}
	{% if j_BITINT_MAXWIDTH >= 128 and not j_is_power_of_two(j_BITINT_MAXWIDTH) %}
		{% do bitint_printers.append(j_BITINT_MAXWIDTH) %}
	{% endif %}
{% endif %}

{# Add types from bitint_printers into printers_types #}
{% for i in bitint_printers %}
	{% if i != 128 %}
		{% do printers_types.append(["ubitint"~i, "unsigned _BitInt("~i~")", "YYIO_BITINT_MAXWIDTH >= "~i]) %}
	{% endif %}
{% endfor %}

{% call(V) j_FOREACHAPPLY(printers_types) %}

{% if V.3 is defined %}
#line
#if $3
{% endif %}
#line

{% if j_match(V.2, "unsigned") %}
#line

static inline
int YYIO_print_$1_inradix(yio_printctx_t *t, $2 arg, bool is_negative,
		char type, $2 radix, char *res, size_t ressize) {
	const char *fmt = YYIO_digit_to_hexs(type == 'x');
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
		case 'u':
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
	const int err = yio_printctx_init_or_number(t, arg);
	if (err) return err;
	return YYIO_print_$1_in(t, arg, false);
}

{% else %}
#line

int YYIO_print_$1(yio_printctx_t *t) {
	const $2 arg = yio_printctx_va_arg_promote(t, $2);
	const int err = yio_printctx_init_or_number(t, arg);
	if (err) return err;
	const bool is_negative = arg < 0;
	typedef unsigned $2 unsignedtype;
	const unsignedtype uarg = is_negative ? -((unsignedtype)arg) : (unsignedtype)arg;
	return YYIO_print_u$1_in(t, uarg, is_negative);
}

{% endif %}

{% if V.3 is defined %}
#endif // $3
{% endif %}
{% endcall %}

#define LOG2(n)  ((n) < 2 ? 0 : (n) < 4 ? 1 : (n) < 8 ? 2 : (n) < 16 ? 3 : (n) < 32 ? 4 : (n) < 64 ? 5 : (n) < 128 ? 6 : (n) < 256 ? 7 : (n) < 512 ? 8 : (n) < 1024 ? 9 : (n) < 2048 ? 10 : (n) < 4096 ? 11 : (n) < 8192 ? 12 : (n) < 16384 ? 13 : (n) < 32768 ? 14 : (n) < 65536 ? 15 : 31)
#define WIDTH_OF(x) (LOG2(x) + 1)

#if __BITINT_MAXWIDTH__

{% set bitint_types = [[1, 'unsigned']] %}
{% for i in j_one_to_n(2, j_BITINT_MAXWIDTH) %}
    {% do bitint_types.append([i, '']) %}
    {% do bitint_types.append([i, 'unsigned']) %}
{% endfor %}

{% call(V) j_FOREACHAPPLY(bitint_types) %}

int YYIO_print_{{ V.2[0:1] }}bitint$1(yio_printctx_t *t) {
	typedef $2 _BitInt($1) T;
	const T arg = yio_printctx_va_arg_promote(t, T);
	const int err = yio_printctx_init_or_number(t, arg);
	if (err) return err;
	const bool is_negative = {% if V.2 == 'unsigned' %} 0 {% else %} arg < 0 {% endif %} ;
	typedef unsigned _BitInt($1) unsignedtype;
	const unsignedtype uarg = is_negative ? -((unsignedtype)arg) : (unsignedtype)arg;
	return
#if WIDTH_OF(UINT_MAX) >= $1
		YYIO_print_uint_in
#elif WIDTH_OF(ULONG_MAX) >= $1
		YYIO_print_ulong_in
#elif WIDTH_OF(ULLONG_MAX) >= $1
		YYIO_print_ullong_in
#elif 128 >= $1 && YYIO_HAS_INT128
		YYIO_print_uint128_in
#elif 128 >= $1
		YYIO_print_bitint128_in
{# for each bitint printer, choose appriopriate one for the bitint size. #}
{% for i in bitint_printers %}
#elif {{i}} >= $1
		YYIO_print_ubitint{{i}}_in
{% endfor %}
#else
#error No idea how to print _BitInt($1)
#endif
		(t, uarg, is_negative);
}

{% endcall %}
#endif // __BITINT_MAXWIDTH__

