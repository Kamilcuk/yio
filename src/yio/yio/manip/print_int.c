/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config();
#include "private.h"
#include <ctype.h>

/* ------------------------------------------------------------------------------- */

static inline
int _yIO_printint_to_radix(char type) {
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
const Ychar *_yIO_printint_to_fmt(Ychar type) {
	return Yisupper(type) ?
			Yc("0123456789ABCDEF") :
			Yc("0123456789abcdef");
}

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

#define _yIO_printint_G(SUFFIX, TYPE) \
int _yIO_print_ ## SUFFIX(yio_printctx_t *t) { \
	TYPE arg = yio_printctx_va_arg_num(t, TYPE); \
	Ychar res[sizeof(TYPE) * CHAR_BIT + 1] = {0}; \
	\
	Ychar * num = res + sizeof(res); \
	(--num)[0] = '\0'; \
	const Ychar type = yio_printctx_get_fmt(t)->type; \
	const TYPE radix = _yIO_printint_to_radix(type); \
	const Ychar *format = _yIO_printint_to_fmt(type); \
	const bool negative = arg < 0; \
	assert(res != num); \
	const TYPE tmp = arg % radix; \
	(--num)[0] = format[negative ? -tmp : tmp]; \
	if (arg /= radix) { \
		if (negative) { \
			arg = -arg; \
		} \
		do { \
			(--num)[0] = format[arg % radix]; \
		} while (arg /= radix); \
	} \
	return _yIO_printformat_number(t, num, !negative); \
}

_yIO_APPLYFOREACHUNPACK(_yIO_printint_G, _yIO_INTEGERS_LIST2())
