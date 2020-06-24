/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config(yio);
#include "private.h"
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
	replacement_field ::=  "{" [field_name] ["!" conversion] [":" format_spec] "}"
	field_name        ::=  arg_name ("." attribute_name | "[" element_index "]")*
	arg_name          ::=  [identifier | digit+]
	attribute_name    ::=  identifier
	element_index     ::=  digit+ | index_string
	index_string      ::=  <any source character except "]"> +
	conversion        ::=  "r" | "s" | "a"
	format_spec       ::=  <described in the next section>
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

	*pf = _yIO_printfmt_default;

	if (fmt[0] == '{') {
		fmt++;
	}
	if (fmt[0] == '}') {
		fmt++;
		goto EXIT;
	}
	if (fmt[0] != ':') {
		ret = YIO_ERROR_PYFMT_INVALID;
		goto EXIT;
	}
	++fmt;
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
		const char *endptr;
		err = _yIO_printctx_stdintparam(c, fmt, &endptr, &pf->precision);
		if (err) {
			ret = err;
			goto EXIT;
		}
		// If there is a dot, there must be precision.
		if (endptr == fmt) {
			ret = YIO_ERROR_PYFMT_INVALID;
			goto EXIT;
		}
		fmt = endptr;
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

	*pf = _yIO_printfmt_default;

	// if first char is % ignore it
	if (fmt[0] == Yc('%')) {
		++fmt;
	}

	// conversion flag specifier
	for (; fmt[0] != Yc('\0'); ++fmt) {
		bool dobreak = false;
		switch (fmt[0]) {
		case Yc('+'): pf->sign = Yc('+'); break;
		case Yc('-'): pf->sign = Yc('-'); break;
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

		int err = _yIO_printctx_stdintparam(c, fmt, &fmt, &pf->precision);
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
	pf->type = _yIO_strnulchrbool(Yc("bcdeEfFgGnosxX"), fmt[0]) ? fmt++[0] : 0;

	EXIT:
	*endptr = fmt;
	return ret;
}

/* --------------------------------------------------------------------------- */

typedef struct _yIO_printformat_t {
	yio_printctx_t *t;
	size_t str_len;
	size_t alllen;
	bool is_number;
	bool is_positive;
} _yIO_printformat_t;

/**
 * Construct print formatting options.
 * @param t Yio printing context.
 * @param str_len The length of the string to be printed.
 * @param is_number If the output is a number.
 * When true, that means that the output is a floating point number.
 * That means that it is composed of only digits with an optional single
 * full stop point.
 * When true, the function will apply thousends separator and
 * properly format the sign.
 * @param is_positive If the number to be printed is positive or negative.
 * Only important if @c is_number is true.
 * The string to be printed must be passed without sign!!
 * @return
 */
static inline
_yIO_printformat_t _yIO_printformat_init(yio_printctx_t *t,
		size_t str_len, bool is_number, bool is_positive) {
	_yIO_printformat_t ret = {
			.t = t,
			.str_len = str_len,
			.is_number = is_number,
			.is_positive = is_positive,
	};
	return ret;
}

static inline
int _yIO_printformat_prefix__print_sign_hash(yio_printctx_t *t,
		const struct yio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const char c = is_positive ? f->sign : Yc('-');
		const int err = yio_printctx_out(t, &c, 1);
		if (err) return err;
	}
	if (has_hash) {
		assert(Yisprint(f->type));
		const Ychar buf[2] = { Yc('0'), f->type, };
		const int err = yio_printctx_out(t, buf, sizeof(buf));
		if (err) return err;
	}
	return 0;
}

static inline
int _yIO_printformat_prefix(_yIO_printformat_t *pf) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const bool is_positive = pf->is_positive;
	size_t * const alllen0 = &pf->alllen;
	const size_t len = pf->str_len;

	const bool has_hash = is_number && f->hash && Ystrchr(Yc("xXoObB"), f->type) != NULL;
	const bool has_sign = is_number && (f->sign == YIO_SIGN_ALWAYS ||
					f->sign == YIO_SIGN_ALWAYSSPACE || is_positive == false);
	const size_t alllen = len + 2 * has_hash + has_sign;
	*alllen0 = alllen;
	const size_t width = f->width > 0 ? f->width : 0;

	if (f->align == YIO_ALIGN_PADSIGN) {
		const int err = _yIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	if ((f->align == YIO_ALIGN_PADSIGN ||
			f->align == YIO_ALIGN_RIGHT ||
			f->align == YIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = width - alllen;
		size_t diff = f->align == YIO_ALIGN_CENTER ? tmp / 2 + !!(tmp % 2) : tmp;
		while (diff--) {
			const int err = yio_printctx_out(t, &f->fill, 1);
			if (err) return err;
		}
	}

	if (f->align != YIO_ALIGN_PADSIGN) {
		const int err = _yIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	return 0;
}

static inline
int _yIO_printformat_suffix(_yIO_printformat_t *pf) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const size_t alllen = pf->alllen;

	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == YIO_ALIGN_LEFT || f->align == YIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == YIO_ALIGN_CENTER ? tmp / 2: tmp;
		while (diff--) {
			const int err = yio_printctx_out(t, &f->fill, 1);
			if (err) return err;
		}
	}
	return 0;
}

static inline
const char *_yIO_str_comma_or_end(const Ychar str[]) {
	for (; str[0] != Yc('\0') && str[0] != Yc('.'); ++str) {
		continue;
	}
	return str;
}

static inline
int _yIO_print_format_generic_number_grouping(yio_printctx_t *t, Ychar grouping,
		const Ychar str[], size_t str_len) {

	assert(Yisprint((unsigned char)grouping));
	int err = 0;
	// Find end of string (integer) or comma (floating point)
	const Ychar * const str0 = str;
	const Ychar *comma_or_end = _yIO_str_comma_or_end(str);
	// Calculate size of first block.
	const size_t blocksize = 3;
	const size_t numberscnt = comma_or_end - str;
	const size_t firstblock = numberscnt % blocksize;
	// _yIO_dbgln("numberscnt=%zu firstblock=%zu", numberscnt, firstblock);

	// Print nn_nnn_nnn_nnn_nnn_nnn[.nnnnnnnn]
	if (firstblock) {
		// print the first non divisible by blocksize
		err = yio_printctx_out(t, str, firstblock);
		str += firstblock;
		if (err) return err;
		// if there are more numbers to write, write the grouping character
		if (str != comma_or_end) {
			err = yio_printctx_out(t, &grouping, 1);
			if (err) return err;
		}
	}
	// if there are more characters to write
	if (str != comma_or_end) {
		while (1) {
			// we get here either by writing first block and printing grouping
			// or by printing nothing by far
			// or by the loop
			err = yio_printctx_out(t, str, blocksize);
			str += blocksize;
			if (err) return err;
			// we check if the end is between grouping and end
			assert(str <= comma_or_end);
			if (str == comma_or_end) {
				break;
			}
			// print the grounping between
			err = yio_printctx_out(t, &grouping, 1);
			if (err) return err;
		}
	}

	assert(str >= str0);
	const size_t written_so_far = str - str0;
	assert(str_len >= written_so_far);
	const size_t left_to_write = str_len - written_so_far;
	if (left_to_write) {
		err = yio_printctx_out(t, str, left_to_write);
		if (err) return err;
	}

	return err;
}

static inline
int _yIO_printformat_print(_yIO_printformat_t *pf, const Ychar str[]) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const Ychar grouping = f->grouping;
	const size_t str_len = pf->str_len;

	if (is_number == true && grouping != YIO_GROUPING_NONE) {
		const int err = _yIO_print_format_generic_number_grouping(t, grouping, str, str_len);
		if (err) return err;
	} else {
		const int precision = f->precision;
		const size_t to_print =
				is_number == true || precision < 0 || (size_t)precision >= str_len ?
						str_len : (size_t)precision;
		const int err = yio_printctx_out(t, str, to_print);
		if (err) return err;
	}

	return 0;
}

static inline
int _yIO_printformat_generic(yio_printctx_t * restrict t,
		const char str[restrict], bool is_number, bool is_positive) {
	_yIO_printformat_t pf = _yIO_printformat_init(t,
			strlen(str), is_number, is_positive);
	int err = _yIO_printformat_prefix(&pf);
	if (err) return err;
	err = _yIO_printformat_print(&pf, str);
	if (err) return err;
	err = _yIO_printformat_suffix(&pf);
	if (err) return err;
	return 0;
}

int _yIO_printformat_number(yio_printctx_t *t, const Ychar str[], bool is_positive) {
	return _yIO_printformat_generic(t, str, true, is_positive);
}

int _yIO_printformat_string(yio_printctx_t *t, const Ychar str[]) {
	return _yIO_printformat_generic(t, str, 0, 0);
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

