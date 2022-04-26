/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "private.h"
#include "ctx.h"
#include "fmt_private.h"
#include "ctx_private.h"
#include "yio/yio_error.h"
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

static inline
int _yΩIO_digit_to_number(TCHAR d) {
	assert(TISDIGIT(d));
{% if MODEX == 1 %}
	return d - '0';
{% else %}
	const TCHAR table[] = TC("0123456789");
	return TSTRCHR(table, d) - table;
{% endif %}
#line
}

int _yΩIO_printctx_strtoi_noerr(const TCHAR **ptr) {
	const TCHAR *pnt = *ptr;
	int num = 0;
	do {
		assert(num < INT_MAX / 10);
		num *= 10;
		const int c = _yΩIO_digit_to_number(pnt[0]);
		assert(num < INT_MAX - c);
		num += c;
		++pnt;
	} while (TISDIGIT(pnt[0]));
	*ptr = pnt;
	return num;
}

int _yΩIO_printctx_stdintparam(yπio_printctx_t *t,
		const TCHAR *ptr, const TCHAR **endptr, int *res) {
	int num = -1;
	int ret = 0;
	if (ptr[0] == TC('{')) {
		ptr++;
		if (TISDIGIT(ptr[0])) {
#if 1 || _yIO_USE_POSITIONAL
			_yΩIO_skip_arm(t, _yΩIO_printctx_strtoi_noerr(&ptr));
			const int skiperr = _yΩIO_skip_do(t);
			if (skiperr) return skiperr;
#else
			return YIO_ERROR_POSITIONAL_DISABLED;
#endif
		}
		const _yΩIO_printfunc_t ifunc = *t->ifunc++;
		if (ifunc == &_yΩIO_print_short)       { num = yπio_printctx_va_arg_num(t, short); }
		else if (ifunc == &_yΩIO_print_ushort) { num = yπio_printctx_va_arg_num(t, unsigned short); }
		else if (ifunc == &_yΩIO_print_int)    { num = yπio_printctx_va_arg(t, int); }
		else if (ifunc == &_yΩIO_print_uint)   { num = yπio_printctx_va_arg(t, unsigned int); }
		else if (ifunc == &_yΩIO_print_long)   { num = yπio_printctx_va_arg(t, long); }
		else if (ifunc == &_yΩIO_print_ulong)  { num = yπio_printctx_va_arg(t, unsigned long); }
		else if (ifunc == &_yΩIO_print_llong)  { num = yπio_printctx_va_arg(t, long long); }
		else if (ifunc == &_yΩIO_print_ullong) { num = yπio_printctx_va_arg(t, unsigned long long); }
		else { return YIO_ERROR_NOT_NUMBER; }
		if (ptr++[0] != TC('}')) {
			ret = YIO_ERROR_MISSING_RIGHT_BRACE;
			goto EXIT;
		}
	} else if (TISDIGIT(ptr[0])) {
		num = _yΩIO_printctx_strtoi_noerr(&ptr);
	}
	EXIT:
	*endptr = ptr;
	*res = num;
	return ret;
}

const struct yπio_printfmt_s _yΩIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = TC(' '),
		.align = TC('>'),
		.sign = TC('-'),
};

bool _yΩIO_strnulchrbool(const TCHAR *s, TCHAR c) {
	return c != TC('\0') && TSTRCHR(s, c) != NULL;
}

int _yΩIO_pfmt_parse(struct _yΩIO_printctx_s *c, struct yπio_printfmt_s *pf,
		const TCHAR *fmt, const TCHAR **endptr) {
	/*
	https://fmt.dev/latest/syntax.html#format-specification-mini-language
	format_spec     ::=  [[fill]align][sign][#][0][width][grouping_option][.precision][type]
	fill            ::=  <any character>
	align           ::=  "<" | ">" | "=" | "^"
	sign            ::=  "+" | "-" | " "
	width           ::=  digit+
	grouping_option ::=  "_" | ","
	precision       ::=  digit+
	type            ::=  "b" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | "o" | "s" | "x" | "X" | "%"
	 */
	int ret = 0;

	if (fmt[0] == TC('}')) {
		fmt++;
		goto EXIT;
	}
	if (fmt[0] == TC('\0')) {
		ret = YIO_ERROR_EOF_IN_FMT;
		goto EXIT;
	}
	if (fmt[0] != TC('\0') && _yΩIO_strnulchrbool(TC("<>=^"), fmt[1])) {
		pf->fill = fmt++[0];
		pf->align = fmt++[0];
	} else if (_yΩIO_strnulchrbool(TC("<>=^"), fmt[0])) {
		pf->align = fmt++[0];
	}
	if (_yΩIO_strnulchrbool(TC("+- "), fmt[0])) {
		pf->sign = fmt++[0];
	}
	if (fmt[0] == TC('#')) {
		pf->hash = true;
		fmt++;
	}
	if (fmt[0] == TC('0')) {
		fmt++;
		pf->fill = TC('0');
		pf->align = TC('=');
	}

	int err = _yΩIO_printctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	pf->grouping = _yΩIO_strnulchrbool(TC("_,"), fmt[0]) ? fmt++[0] : 0;
	if (fmt[0] == '.') {
		++fmt;
		const TCHAR *endparamptr;
		err = _yΩIO_printctx_stdintparam(c, fmt, &endparamptr, &pf->precision);
		if (err) {
			ret = err;
			goto EXIT;
		}
		// If there is a dot, there must be precision.
		if (endparamptr == fmt) {
			ret = YIO_ERROR_MISSING_PRECISION;
			goto EXIT;
		}
		fmt = endparamptr;
	} else {
		pf->precision = -1;
	}

	pf->type = _yΩIO_strnulchrbool(TC("bcdeEfFaAgGnosxX%"), fmt[0]) ? fmt++[0] : 0;

	if (fmt[0] != '}') {
		ret = YIO_ERROR_MISSING_RIGHT_BRACE;
		goto EXIT;
	}
	++fmt;

	EXIT:
	*endptr = fmt;
	return ret;
}

