/**
 * @file yio_fmt.c
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio_private.h"
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

const struct yio_printfmt_s _yIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = ' ',
		.align = '>',
		.sign = '-',
};

const struct yio_scanfmt_s _yIO_scanfmt_default = {
		.width = -1,
		.set = NULL,
		.ignore = 0,
};

void _yIO_printfmt_println(const struct yio_printfmt_s *t) {
	fprintf(stderr,
			"width:% d precision:% d "
			"fill:%02x(%c) align:%02x(%c) sign:%02x(%c) "
			"hash:%02x(%c) group:%02x(%c) type:%02x(%c)"
			"\n"
			,
			t->width, t->precision,
#define _(x)  x, isprint((unsigned char)x) ? x : '.'
			_(t->fill),
			_(t->align),
			_(t->sign),
			_(t->hash),
			_(t->grouping),
			_(t->type)
#undef _
	);
}

int _yIO_pfmt_parse(struct _yIO_commonctx_s *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr) {
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
	const bool has_fill = fmt[1] != '\0' && strchr("<>=^", fmt[1]) != NULL;
	pf->fill = has_fill ? fmt++[0] : ' ';
	pf->align = (fmt[0] != '\0' && strchr("<>=^", fmt[0]) != NULL) ? fmt++[0] : '>';
	pf->sign = (fmt[0] != '\0' && strchr("+- ", fmt[0]) != NULL) ? fmt++[0] : '-';
	pf->hash = fmt[0] == '#' ? fmt++, 1 : 0;
	if (fmt[0] == '0') {
		fmt++;
		pf->fill = '0';
		pf->align = '=';
	}

	int err = _yIO_commonctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	pf->grouping = (fmt[0] != '\0' && strchr("_,", fmt[0]) != NULL) ? fmt++[0] : 0;
	if (fmt[0] == '.') {
		++fmt;
		const char *endptr;
		err = _yIO_commonctx_stdintparam(c, fmt, &endptr, &pf->precision);
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

	pf->type = (fmt[0] != '\0' && strchr("bcdeEfFgGnosxX%", fmt[0]) != NULL) ? fmt++[0] : 0;

	if (fmt[0] != '}') {
		ret = YIO_ERROR_PYFMT_INVALID;
		goto EXIT;
	}
	++fmt;

	EXIT:
	*endptr = fmt;
	return ret;
}

int _yIO_cfmt_parse(struct _yIO_commonctx_s *c, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr)  {
	// regex:
	// [%]?[+- #0]*([0-9]*|\*)?(\.([0-9]*|\*)?)?
	//     actually precision_plus_1 is needed after comma, but anyway
	// ([l]?c|[l]?s|[hl]{0,2}[dioxXu]|[lL]?[fFeEaAgG]|n|p)
	int ret = 0;

	*pf = _yIO_printfmt_default;

	if (fmt == NULL) {
		goto EXIT;
	}

	// if first char is % ignore it
	if (fmt[0] == '%') {
		++fmt;
	}

	// conversion flag specifier
	for (; fmt[0] != '\0'; ++fmt) {
		bool dobreak = false;
		switch (fmt[0]) {
		case '+': pf->sign = '+'; break;
		case '-': pf->sign = '-'; break;
		case ' ': pf->fill = ' '; break;
		case '#': pf->hash = '#'; break;
		case '0': pf->fill = '0'; break;
		default: dobreak = true; break;
		}
		if (dobreak) {
			break;
		}
	}

	// minimum field width
	int err = _yIO_commonctx_stdintparam(c, fmt, &fmt, &pf->width);
	if (err) {
		ret = err;
		goto EXIT;
	}

	// precision_plus_1
	if (*fmt == '.') {
		fmt++;

		int err = _yIO_commonctx_stdintparam(c, fmt, &fmt, &pf->precision);
		if (err) {
			ret = err;
			goto EXIT;
		}
	} else {
		pf->precision = -1;
	}

	// ignore length modifier if any
	while (strchr("ahlLm", *fmt) != NULL) {
		fmt++;
	}

	// conversion format specifier
	pf->type = strchr("bcdeEfFgGnosxX", fmt[0]) ? fmt++[0] : 0;

	EXIT:
	*endptr = fmt;
	return ret;
}

/* --------------------------------------------------------------------------- */

static inline
int _yIO_print_format_from_buf__print_sign_hash(yio_printctx_t *t,
		const struct yio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const char c = is_positive ? f->sign : '-';
		const int err = yio_printctx_out(t, &c, 1);
		if (err) return err;
	}
	if (has_hash) {
		assert(isprint((unsigned char)f->type));
		const char buf[2] = { '0', f->type, };
		const int err = yio_printctx_out(t, buf, sizeof(buf));
		if (err) return err;
	}
	return 0;
}

static inline
int _yIO_print_format_prefix(yio_printctx_t *t,
		const struct yio_printfmt_s *f,
		bool is_number, bool is_positive,
		size_t len, size_t *alllen0) {
	const bool has_hash = is_number && f->hash && (
			f->type == 'x' || f->type == 'X' ||
			f->type == 'o' || f->type == 'O' ||
			f->type == 'b' || f->type == 'B' );
	const bool has_sign = is_number &&
			(f->sign == '+' || f->sign == ' ' || is_positive == false);
	const size_t alllen = len + 2 * has_hash + has_sign;
	*alllen0 = alllen;
	const size_t width = f->width > 0 ? f->width : 0;

	if (f->align == '=') {
		const int err = _yIO_print_format_from_buf__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	if ((f->align == '=' || f->align == '>' || f->align == '^') && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == '^' ? tmp / 2 + !!(tmp % 2) : tmp;
		while (diff--) {
			const int err = yio_printctx_out(t, &f->fill, 1);
			if (err) return err;
		}
	}

	if (f->align != '=') {
		const int err = _yIO_print_format_from_buf__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	return 0;
}

static inline
int _yIO_print_format_suffix(yio_printctx_t *t,
		const struct yio_printfmt_s *f, size_t alllen) {
	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == '<' || f->align == '^') && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == '^' ? tmp / 2 : tmp;
		while (diff--) {
			const int err = yio_printctx_out(t, &f->fill, 1);
			if (err) return err;
		}
	}
	return 0;
}

static inline
const char *_yIO_str_comma_or_end(const char str[]) {
	for (; str[0] != '\0' && str[0] != '.'; ++str) {
		continue;
	}
	return str;
}

static inline
int _yIO_print_format_generic_number_grouping(yio_printctx_t *t, char grouping,
		const char str[], size_t str_len) {
	assert(isprint((unsigned char)grouping));
	int err = 0;
	// Find end of string (integer) or comma (floating point)
	const char * const str0 = str;
	const char *comma_or_end = _yIO_str_comma_or_end(str);
	// Calculate size of firts block;
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
int _yIO_print_format_generic(yio_printctx_t * restrict t, const char str[restrict],
		bool is_number, bool is_positive) {
	// _yIO_dbgln("is_number=%d is_positive=%d str='%s'", is_number, is_positive, str);
	const struct yio_printfmt_s * restrict f = &t->pf;
	const size_t str_len = strlen(str);
	size_t alllen;
	int err = _yIO_print_format_prefix(t, f, is_number, is_positive, str_len, &alllen);
	if (err) return err;
	if (is_number == true && f->grouping != '\0') {
		err = _yIO_print_format_generic_number_grouping(t, f->grouping, str, str_len);
		if (err) return err;
	} else {
		err = yio_printctx_out(t, str, str_len);
		if (err) return err;
	}
	err = _yIO_print_format_suffix(t, f, alllen);
	if (err) return err;
	return 0;
}

int _yIO_print_format_number(yio_printctx_t *t, const char str[], bool is_positive) {
	return _yIO_print_format_generic(t, str, true, is_positive);
}

int _yIO_print_format_string(yio_printctx_t *t, const char str[]) {
	return _yIO_print_format_generic(t, str, 0, 0);
}

/* --------------------------------------------------------------------------------- */

int _yIO_print_cfmt(yio_printctx_t *t) {
	const char *str = yio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	int err = _yIO_cfmt_parse(&t->c, &t->pf, str, &endptr);
	if (err) return err;
	if (endptr != strchr(str, '\0')) {
		return YIO_ERROR_CFMT_INVALID;
	}
	return yio_printctx_next(t);
}

int _yIO_print_pfmt(yio_printctx_t *t) {
	const char *str = yio_printctx_va_arg(t, const char *);
	const char *endptr = NULL;
	int err = _yIO_pfmt_parse(&t->c, &t->pf, str, &endptr);
	if (err) return err;
	if (endptr != strchr(str, '\0')) {
		return YIO_ERROR_PYFMT_INVALID;
	}
	return yio_printctx_next(t);
}

/* --------------------------------------------------------------------------------- */

int _yIO_scan_parse_scanfmt(struct _yIO_commonctx_s *c, struct yio_scanfmt_s *sf,
		const char *fmt, const char **endptr) {
	// {[*][width][iduoxfegacspn]}
	int err = 0;
	*sf = _yIO_scanfmt_default;
	if (fmt[0] != '{') {
		err = YIO_ERROR_SCANFMT_INVALID;
		goto EXIT;
	}
	++fmt;
	sf->ignore = fmt[0] == '*' ? fmt++, '*' : 0;
	err = _yIO_commonctx_stdintparam(c, fmt, &fmt, &sf->width);
	if (err) {
		goto EXIT;
	}
	sf->type = (fmt[0] != '\0' && strchr("iduoxfegacspn", fmt[0]) != NULL) ? fmt++[0] : 0;
	if (fmt[0] != '}') {
		err = YIO_ERROR_SCANFMT_INVALID;
		goto EXIT;
	}
	++fmt;
	EXIT:
	*endptr = fmt;
	return err;
}

