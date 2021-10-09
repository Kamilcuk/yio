/**
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

/* ------------------------------------------------------------------------------- */

static inline
int _yΩIO_printint_to_radix(Ychar type) {
	switch (type) {
	case 0:
	case Yc('d'):
		return 10;
	case Yc('x'):
	case Yc('X'):
		return 16;
	case Yc('o'):
		return 8;
	case Yc('b'):
		return 2;
	default:
		assert(!"Invalid print type");
	}
	return 10;
}


static inline
const Ychar *_yΩIO_printint_to_fmt(Ychar type) {
	return Yisupper(type) ?
			Yc("0123456789ABCDEF") :
			Yc("0123456789abcdef");
}

m4_applyforeachdefine((
	(short, short),
	(ushort, unsigned short),
	(int, int),
	(uint, unsigned int),
	(long, long),
	(ulong, unsigned long),
	(llong, long long),
	(ullong, unsigned long long),
	(__int128, __int128, _yIO_HAS_INT128),
	(u__int128, unsigned __int128, _yIO_HAS_INT128)
), m4_syncline(1)«m4_dnl;

m4_ifelse(«$3»,«»,«», m4_syncline(1)«m4_dnl;
#ifndef $3
#error
#endif
#if $3
») m4_syncline(1) m4_dnl;

int _yΩIO_print_$1(yπio_printctx_t *t) {
	$2 arg = yπio_printctx_va_arg_num(t, $2);
	int err = yπio_printctx_init(t);
	if (err) return err;

	Ychar res[sizeof($2) * CHAR_BIT + 1] = {0};
	Ychar * const res_end = res + sizeof(res)/sizeof(*res);
	Ychar * num = res_end;
	const Ychar type = yπio_printctx_get_fmt(t)->type;
	const $2 radix = _yΩIO_printint_to_radix(type);
	const Ychar *format = _yΩIO_printint_to_fmt(type);
	const bool negative =
m4_ign("If $2 is unsigned, it can't be negative"
		"So remove the check, so that -Wtype-limits doesn't throw") m4_dnl;
m4_ifelse(m4_regexp(«$2»,«unsigned»),«-1»,
			arg < 0;
,
			false;
) m4_dnl;
	assert(res != num);
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

m4_ifelse(«$3»,«»,«», m4_syncline(1)«m4_dnl;
#endif // $3
») m4_syncline(1) m4_dnl;

») m4_dnl;

