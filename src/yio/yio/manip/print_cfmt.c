/**
 * @file
 * @date 2020-lip-29
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <ctype.h>

int _yΩIO_cfmt_parse(struct _yΩIO_printctx_s *t, struct yπio_printfmt_s *pf,
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
	if (Yisdigit(fmt[0])) {
		pf->width = _yΩIO_commonctx_strtoi_noerr(&fmt);
	} else if (fmt[0] == Yc('*')) {
		pf->width = yπio_printctx_va_arg(t, int);
	}

	// precision
	if (*fmt == Yc('.')) {
		fmt++;
		if (Yisdigit(fmt[0])) {
			pf->precision = _yΩIO_commonctx_strtoi_noerr(&fmt);
		} else if (fmt[0] == Yc('*')) {
			pf->precision = yπio_printctx_va_arg(t, int);
		} else {
			pf->precision = -1;
		}
	} else {
		pf->precision = -1;
	}

	// ignore length modifier if any
	while (_yΩIO_strnulchrbool(Yc("ahlLm"), *fmt)) {
		fmt++;
	}

	// conversion format specifier
	pf->type = _yΩIO_strnulchrbool(Yc("csdioxXufFeEaAgGnp"), fmt[0]) ? fmt++[0] : 0;

	*endptr = fmt;
	return ret;
}

int _yΩIO_print_cfmt(yπio_printctx_t *t) {
	const char *str = yπio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	int err = _yΩIO_cfmt_parse(t, &t->pf, str, &endptr);
	if (err) return err;
	if ((size_t)(endptr - str) != Ystrlen(str)) {
		return YIO_ERROR_CFMT_INVALID;
	}
	return yπio_printctx_next(t);
}

