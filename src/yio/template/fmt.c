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
int _yΩIO_printctx_stdintparam_callback(void *arg) {
	struct _yΩIO_printctx_s *t = arg;
	++t->ifunc;
	return yπio_printctx_va_arg(t, int);
}

static inline
int _yΩIO_printctx_stdintparam(struct _yΩIO_printctx_s *t, const Ychar *ptr, const Ychar **endptr, int *res) {
	return _yΩIO_commonctx_stdintparam(_yΩIO_printctx_stdintparam_callback, t, ptr, endptr, res);
}

const struct yπio_printfmt_s _yΩIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = Yc(' '),
		.align = Yc('>'),
		.sign = Yc('-'),
};

bool _yΩIO_strnulchrbool(const Ychar *s, Ychar c) {
	return c != Yc('\0') && Ystrchr(s, c) != NULL;
}

int _yΩIO_pfmt_parse(struct _yΩIO_printctx_s *c, struct yπio_printfmt_s *pf,
		const Ychar *fmt, const Ychar **endptr) {
	/*
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

	if (fmt[0] == '}') {
		fmt++;
		goto EXIT;
	}
	if (fmt[0] == '\0') {
		ret = YIO_ERROR_EOF_IN_FMT;
		goto EXIT;
	}
	const bool has_fill = _yΩIO_strnulchrbool(Yc("<>=^"), fmt[1]);
	pf->fill = has_fill ? fmt++[0] : Yc(' ');
	pf->align = _yΩIO_strnulchrbool(Yc("<>=^"), fmt[0]) ? fmt++[0] : Yc('>');
	pf->sign = _yΩIO_strnulchrbool(Yc("+- "), fmt[0]) ? fmt++[0] : Yc('-');
	pf->hash = fmt[0] == Yc('#') ? fmt++, true : false;
	if (fmt[0] == Yc('0')) {
		fmt++;
		pf->fill = Yc('0');
		pf->align = Yc('=');
	}

	int err = _yΩIO_printctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	pf->grouping = _yΩIO_strnulchrbool(Yc("_,"), fmt[0]) ? fmt++[0] : 0;
	if (fmt[0] == '.') {
		++fmt;
		const Ychar *endparamptr;
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

	pf->type = _yΩIO_strnulchrbool(Yc("bcdeEfFaAgGnosxX%"), fmt[0]) ? fmt++[0] : 0;

	if (fmt[0] != '}') {
		ret = YIO_ERROR_MISSING_RIGHT_BRACE;
		goto EXIT;
	}
	++fmt;

	EXIT:
	*endptr = fmt;
	return ret;
}

