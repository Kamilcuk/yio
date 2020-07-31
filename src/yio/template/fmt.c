/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config_yio() m4_dnl;
#include "private.h"
#include "ctx.h"
#include "fmt_private.h"
#include "ctx_private.h"
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

static inline
int _yIO_printctx_stdintparam_callback(void *arg) {
	struct _yIO_printctx_s *t = arg;
	++t->ifunc;
	return yio_printctx_va_arg(t, int);
}

static inline
int _yIO_printctx_stdintparam(struct _yIO_printctx_s *t, const Ychar *ptr, const Ychar **endptr, int *res) {
	return _yIO_commonctx_stdintparam(_yIO_printctx_stdintparam_callback, t, ptr, endptr, res);
}

static inline
int _yIO_scanctx_stdintparam_callback(void *arg) {
	struct _yIO_scanctx_s *t = arg;
	++t->ifunc;
	return yio_scanctx_va_arg(t, int);
}

static inline
int _yIO_scanctx_stdintparam(struct _yIO_scanctx_s *t, const Ychar *ptr, const Ychar **endptr, int *res) {
	return _yIO_commonctx_stdintparam(_yIO_scanctx_stdintparam_callback, t, ptr, endptr, res);
}

const struct yio_printfmt_s _yIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = Yc(' '),
		.align = Yc('>'),
		.sign = Yc('-'),
};

const struct yio_scanfmt_s _yIO_scanfmt_default = {
		.width = -1,
		.set = NULL,
		.ignore = 0,
};

static inline
bool _yIO_strnulchrbool(const Ychar *s, Ychar c) {
	return c != Yc('\0') && Ystrchr(s, c) != NULL;
}

int _yIO_pfmt_parse(struct _yIO_printctx_s *c, struct yio_printfmt_s *pf,
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
		ret = YIO_ERROR_PYFMT_INVALID;
		goto EXIT;
	}
	const bool has_fill = _yIO_strnulchrbool(Yc("<>=^"), fmt[1]);
	pf->fill = has_fill ? fmt++[0] : Yc(' ');
	pf->align = _yIO_strnulchrbool(Yc("<>=^"), fmt[0]) ? fmt++[0] : Yc('>');
	pf->sign = _yIO_strnulchrbool(Yc("+- "), fmt[0]) ? fmt++[0] : Yc('-');
	pf->hash = fmt[0] == Yc('#') ? fmt++, true : false;
	if (fmt[0] == Yc('0')) {
		fmt++;
		pf->fill = Yc('0');
		pf->align = Yc('=');
	}

	int err = _yIO_printctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	pf->grouping = _yIO_strnulchrbool(Yc("_,"), fmt[0]) ? fmt++[0] : 0;
	if (fmt[0] == '.') {
		++fmt;
		const char *endparamptr;
		err = _yIO_printctx_stdintparam(c, fmt, &endparamptr, &pf->precision);
		if (err) {
			ret = err;
			goto EXIT;
		}
		// If there is a dot, there must be precision.
		if (endparamptr == fmt) {
			ret = YIO_ERROR_PYFMT_INVALID;
			goto EXIT;
		}
		fmt = endparamptr;
	} else {
		pf->precision = -1;
	}

	pf->type = _yIO_strnulchrbool(Yc("bcdeEfFgGnosxX%"), fmt[0]) ? fmt++[0] : 0;

	if (fmt[0] != '}') {
		ret = YIO_ERROR_PYFMT_INVALID;
		goto EXIT;
	}
	++fmt;

	EXIT:
	*endptr = fmt;
	return ret;
}

int _yIO_cfmt_parse(struct _yIO_printctx_s *c, struct yio_printfmt_s *pf,
		const Ychar *fmt, const Ychar **endptr)  {
	// regex:
	// [%]?[+- #0]*([0-9]*|\*)?(\.([0-9]*|\*)?)?
	//     actually precision_plus_1 is needed after comma, but anyway
	// ([l]?c|[l]?s|[hl]{0,2}[dioxXu]|[lL]?[fFeEaAgG]|n|p)
	int ret = 0;

	// if first char is % ignore it
	if (fmt[0] == Yc('%')) {
		++fmt;
	}

	// conversion flag specifier
	for (; fmt[0] != Yc('\0'); ++fmt) {
		bool dobreak = false;
		switch (fmt[0]) {
		case Yc('+'): pf->sign = Yc('+'); break;
		case Yc('-'): pf->align = Yc('<'); break;
		case Yc(' '): pf->fill = Yc(' '); break;
		case Yc('#'): pf->hash = Yc('#'); break;
		case Yc('0'): pf->fill = Yc('0'); break;
		default: dobreak = true; break;
		}
		if (dobreak) {
			break;
		}
	}

	// minimum field width
	int err = _yIO_printctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	// precision_plus_1
	if (*fmt == Yc('.')) {
		fmt++;

		err = _yIO_printctx_stdintparam(c, fmt, &fmt, &pf->precision);
		if (err) {
			ret = err;
			goto EXIT;
		}
	} else {
		pf->precision = -1;
	}

	// ignore length modifier if any
	while (_yIO_strnulchrbool(Yc("ahlLm"), *fmt)) {
		fmt++;
	}

	// conversion format specifier
	pf->type = _yIO_strnulchrbool(Yc("csdioxXufFeEaAgGnp"), fmt[0]) ? fmt++[0] : 0;

	EXIT:
	*endptr = fmt;
	return ret;
}

/* --------------------------------------------------------------------------------- */

int _yIO_scan_parse_scanfmt(struct _yIO_scanctx_s *c, struct yio_scanfmt_s *sf,
		const char *fmt, const char **endptr) {
	// {[*][width][iduoxfegacspn]}
	int err = 0;
	*sf = _yIO_scanfmt_default;
	if (fmt[0] != Yc('{')) {
		err = YIO_ERROR_SCANFMT_INVALID;
		goto EXIT;
	}
	++fmt;
	sf->ignore = fmt[0] == Yc('*') ? fmt++, Yc('*') : 0;
	err = _yIO_scanctx_stdintparam(c, fmt, &fmt, &sf->width);
	if (err) {
		goto EXIT;
	}
	sf->type = _yIO_strnulchrbool(Yc("iduoxfegacspn"), fmt[0]) ? fmt++[0] : 0;
	if (fmt[0] != Yc('}')) {
		err = YIO_ERROR_SCANFMT_INVALID;
		goto EXIT;
	}
	++fmt;
	EXIT:
	*endptr = fmt;
	return err;
}

