/**
 * @file yio_print_int.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_private.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/* ------------------------------------------------------------------------------- */

static inline
int _yIO_printint_to_radix(char type) {
	switch (type) {
	case 0:
	case 'd':
		return 10;
	case 'x':
	case 'X':
		return 16;
	case 'o':
		return 8;
	case 'b':
		return 2;
	default:
		assert(!"Invalid print type");
	}
	return 10;
}


static inline
const char *_yIO_printint_to_fmt(char type) {
	switch (type) {
	case 0:
	case 'd':
	case 'x':
	case 'o':
	case 'b':
		return "0123456789abcdef";
	case 'X':
		return "0123456789ABCDEF";
	default:
		assert(!"Invalid print type");
	}
	abort();
	return "";
}


#pragma GCC diagnostic ignored "-Wtype-limits"

#define _yIO_printint_G(SUFFIX, TYPE) \
int _yIO_print_ ## SUFFIX(yio_printctx_t *t) { \
	TYPE arg = yio_printctx_va_arg_num(t, TYPE); \
	char res[sizeof(TYPE) * CHAR_BIT + 1] = {0}; \
	\
	char * num = res + sizeof(res); \
	(--num)[0] = '\0'; \
	const char type = yio_printctx_get_fmt(t)->type; \
	const TYPE radix = _yIO_printint_to_radix(type); \
	const char *format = _yIO_printint_to_fmt(type); \
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
	return _yIO_print_format_number(t, num, !negative); \
}

_yIO_APPLYFOREACHUNPACK(_yIO_printint_G, _yIO_INTEGERS_LIST2())
