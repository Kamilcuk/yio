/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#define _XOPEN_SOURCE  1 // wcswidth
#define _POSIX_C_SOURCE  200112L  // nl_langinfo
#define _GNU_SOURCE  1 // GROUPING (?)
#include "ctx.h"
#include "private.h"
#ifndef _yIO_HAS_UNISTRING
#error
#endif
#if _yIO_HAS_UNISTRING
#include <uniwidth.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#ifndef YIO_USE_LOCALE
#error YIO_USE_LOCALE
#endif
#if YIO_USE_LOCALE
#include <langinfo.h>
#endif

/* ------------------------------------------------------------------------- */

static const TCHAR YΩIO_ALIGN_LEFT = TC('<');
static const TCHAR YΩIO_ALIGN_RIGHT = TC('>');
static const TCHAR YΩIO_ALIGN_PADSIGN = TC('=');
static const TCHAR YΩIO_ALIGN_CENTER = TC('^');

static const TCHAR YΩIO_SIGN_ALWAYS = TC('+');
//static const TCHAR YΩIO_SIGN_NEGATIVE = TC('-');
static const TCHAR YΩIO_SIGN_ALWAYSSPACE = TC(' ');

const struct yπio_printfmt_s _yΩIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = TC(' '),
		.align = TC('>'),
		.sign = TC('-'),
};

/* ------------------------------------------------------------------------- */

void _yΩIO_skip_arm(yπio_printctx_t *t, unsigned count) {
	va_end(*t->va);
	va_copy(*t->va, *t->startva);
	t->ifunc = t->startifunc;
	t->skip = count;
}

int _yΩIO_skip_do(yπio_printctx_t *t) {
	for (; t->skip != 0; --t->skip) {
		if (*t->ifunc == NULL) {
			return YIO_ERROR_TOO_MANY_FMT;
		}
		const int ifuncret = (*t->ifunc++)(t);
		assert(ifuncret != 0); // this is not possible
		if (ifuncret != YIO_ERROR_SKIPPING) {
			return ifuncret;
		}
	}
	return 0;
}

static inline
int _yΩIO_digit_to_number(TCHAR d) {
	assert(TISDIGIT(d));
#if TMODEX == 1
	return d - '0';
#else
	const TCHAR table[] = TC("0123456789");
	return TSTRCHR(table, d) - table;
#endif
}

int _yΩIO_printctx_strtoi_noerr(const TCHAR **ptr) {
	const TCHAR *pnt = *ptr;
	assert(TISDIGIT(pnt[0]));
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
			_yΩIO_skip_arm(t, _yΩIO_printctx_strtoi_noerr(&ptr));
			const int skiperr = _yΩIO_skip_do(t);
			if (skiperr) return skiperr;
		}
		if (t->ifunc == NULL) {
			return _yIO_ERROR(YIO_ERROR_POSITIONAL_NO_ARGS, "no arguments for positional width or precision");
		}
		const yπio_printdata_t ifunc = *t->ifunc++;
		if (ifunc == &_yΩIO_print_short)       num = yπio_printctx_va_arg_promote(t, short);
		else if (ifunc == &_yΩIO_print_ushort) num = yπio_printctx_va_arg_promote(t, unsigned short);
		else if (ifunc == &_yΩIO_print_int)    num = yπio_printctx_va_arg(t, int);
		else if (ifunc == &_yΩIO_print_uint)   num = yπio_printctx_va_arg(t, unsigned int);
		else if (ifunc == &_yΩIO_print_long)   num = yπio_printctx_va_arg(t, long);
		else if (ifunc == &_yΩIO_print_ulong)  num = yπio_printctx_va_arg(t, unsigned long);
		else if (ifunc == &_yΩIO_print_llong)  num = yπio_printctx_va_arg(t, long long);
		else if (ifunc == &_yΩIO_print_ullong) num = yπio_printctx_va_arg(t, unsigned long long);
		else return _yIO_ERROR(YIO_ERROR_POSITIONAL_NOT_NUMBER, "positional width or precision specifier is not a number");
		if (ptr++[0] != TC('}')) {
			ret = _yIO_ERROR(YIO_ERROR_POSITIONAL_MISSING_RIGHT_BRACE, "missing '}' when parsing positional width or precision specifier");
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
		ret = _yIO_ERROR(YIO_ERROR_EOF_IN_FMT, "end of string while looking for conversion specifier");
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
			ret = _yIO_ERROR(YIO_ERROR_MISSING_PRECISION, "Format specifier missing precision");
			goto EXIT;
		}
		fmt = endparamptr;
	} else {
		pf->precision = -1;
	}

	if (_yΩIO_strnulchrbool(TC("bcdeEfFaAgGnosxXp%"), fmt[0])) {
		pf->type = fmt++[0];
	}

	if (fmt[0] == TC('\0')) {
		ret = _yIO_ERROR(YIO_ERROR_MISSING_RIGHT_BRACE, "missing '}' when parsing common format specification");
		goto EXIT;
	}
	if (fmt[0] != TC('}')) {
		ret = _yIO_ERROR(YIO_ERROR_ERRONEUS_FORMAT, "erroneus format string");
		goto EXIT;
	}
	++fmt;

	EXIT:
	*endptr = fmt;
	return ret;
}

/* printctx ---------------------------------------------------- */

int yπio_printctx_init(yπio_printctx_t *t) {
	if (t->skip != 0) {
		return YIO_ERROR_SKIPPING;
	}
	if (t->fmt) {
		const int err = _yΩIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt);
		if (err) return err;
	}
	return 0;
}

int yπio_printctx_raw_write(yπio_printctx_t *t, const TCHAR *ptr, size_t size) {
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = (*t->out)(t->outarg, ptr, size);
	if (ret) return ret;
	t->writtencnt += size;
	return 0;
}

int yπio_printctx_next(yπio_printctx_t *t) {
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	if (*t->ifunc == NULL) {
		return _yIO_ERROR(YIO_ERROR_NO_NEXT, "formatting modifier is not followed by an argument");
	}
	return (*t->ifunc)(t);
}

int _yΩIO_printctx_print_in(yπio_printctx_t *t, yπio_printdata_t *data, const TCHAR *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yπvbprintf(t->out, t->outarg, data, fmt, &va);
	va_end(va);
	if (ret < 0) {
		return ret;
	}
	t->writtencnt += ret;
	return 0;
}

/* printformat --------------------------------------------------- */

static inline
size_t _yΩIO_width(const TCHAR *str, size_t str_len) {
#if TMODE == 1 && _yIO_HAS_UNISTRING
	return u8_width((const uint8_t*)str, str_len, locale_charset());
#elif TMODE == 2 && _yIO_HAS_wcswidth
	const int width = wcswidth(str, str_len);
	return width < 0 ? str_len : (size_t)width;
#elif TMODE == 3
	return u16_width(str, str_len, locale_charset());
#elif TMODE == 4
	return u32_width(str, str_len, locale_charset());
#else
	return str_len;
#define HAS_WIDTH  1
#endif
#ifndef HAS_WIDTH
#define HAS_WIDTH  0
#endif
}

typedef struct _yΩIO_printformat_t {
	yπio_printctx_t *t;
	size_t str_len;
	size_t alllen;
	bool is_number;
	bool is_positive;
} _yΩIO_printformat_t;

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
void _yΩIO_printformat_init(_yΩIO_printformat_t *pf, yπio_printctx_t *t,
		const TCHAR *str, size_t str_len, bool is_number, bool is_positive) {
	_yΩIO_printformat_t ret = {
			.t = t,
			.str_len = (is_number ? str_len : _yΩIO_width(str, str_len)),
			.is_number = is_number,
			.is_positive = is_positive,
	};
	*pf = ret;
}

static inline
int _yΩIO_printformat_prefix__print_sign_hash(yπio_printctx_t *t,
		const struct yπio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const TCHAR c = is_positive ? f->sign : TC('-');
		const int err = yπio_printctx_raw_write(t, &c, 1);
		if (err) return err;
	}
	if (has_hash) {
		const TCHAR buf[2] = { TC('0'), f->type, };
		const int err = yπio_printctx_raw_write(t, buf, 2);
		if (err) return err;
	}
	return 0;
}

static inline
int _yΩIO_printformat_prefix(_yΩIO_printformat_t *pf) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const bool is_positive = pf->is_positive;
	size_t * const alllen0 = &pf->alllen;
	const size_t len = pf->str_len;

	const bool has_hash = is_number && f->hash && TSTRCHR(TC("xXoObB"), f->type) != NULL;
	const bool has_sign = is_number && (f->sign == YΩIO_SIGN_ALWAYS ||
					f->sign == YΩIO_SIGN_ALWAYSSPACE || is_positive == false);
	const size_t alllen = len + 2 * has_hash + has_sign;
	*alllen0 = alllen;
	const size_t width = f->width > 0 ? f->width : 0;

	if (f->align == YΩIO_ALIGN_PADSIGN) {
		const int err = _yΩIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	if ((f->align == YΩIO_ALIGN_PADSIGN ||
			f->align == YΩIO_ALIGN_RIGHT ||
			f->align == YΩIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = width - alllen;
		size_t diff = f->align == YΩIO_ALIGN_CENTER ? tmp / 2 : tmp;
		while (diff--) {
			const int err = yπio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}

	if (f->align != YΩIO_ALIGN_PADSIGN) {
		const int err = _yΩIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	return 0;
}

static inline
int _yΩIO_printformat_suffix(_yΩIO_printformat_t *pf) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const size_t alllen = pf->alllen;

	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == YΩIO_ALIGN_LEFT || f->align == YΩIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == YΩIO_ALIGN_CENTER ? tmp / 2 + !!(tmp % 2) : tmp;
		while (diff--) {
			const int err = yπio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}
	return 0;
}

static inline
const TCHAR *str_dot_or_end(const TCHAR str[], size_t len) {
	for (; len-- && str[0] != TC('.'); ++str) {
		continue;
	}
	return str;
}

#if YIO_USE_LOCALE
static const char NOGROUP[1] = { CHAR_MAX };
#endif
static const char GROUP3[2] = "\x03";
static const char GROUP4[2] = "\x04";

static inline
int _yΩIO_print_format_generic_number_grouping(yπio_printctx_t *t, TCHAR grouping,
		const TCHAR str[], size_t str_len) {
	int err = 0;
	// Find end of string (integer) or comma (floating point)
	const TCHAR *const str0 = str;
	const TCHAR *comma_or_end = _yΩIO_str_comma_or_end(str, str_len);
	// Calculate size of first block.
	const size_t blocksize = 3;
	const size_t numberscnt = comma_or_end - str;
	const size_t firstblock = numberscnt % blocksize;
	// _yIO_dbgln("numberscnt=%zu firstblock=%zu", numberscnt, firstblock);

	// Print nn_nnn_nnn_nnn_nnn_nnn[.nnnnnnnn]
	if (firstblock) {
		// print the first non divisible by blocksize
		err = yπio_printctx_raw_write(t, str, firstblock);
		str += firstblock;
		if (err) return err;
		// if there are more numbers to write, write the grouping character
		if (str != comma_or_end) {
			err = yπio_printctx_raw_write(t, &grouping, 1);
			if (err) return err;
		}
	}
#endif
	return TSTRCHR(TC("bBoO"), t->pf.conversion) != NULL ? GROUP3 : GROUP4;
}

struct numsep {
	const TCHAR *sep;
	size_t len;
};

static const TCHAR DEFAULT_THOUSEND_SEP[1] = { TC(',') };

static inline
int print_numsep(yπio_printctx_t *t, struct numsep *ns) {
	if (ns->sep == NULL) {
		if (t->pf.grouping == TC('L')) {
#if YIO_USE_LOCALE
			const char *sep = nl_langinfo(THOUSEP); // THOUSEND_SEP
			if (sep == NULL) {
				ns->sep = DEFAULT_THOUSEND_SEP;
				ns->len = 0;
			} else {
#if TMODEX == 1
				ns->sep = sep;
				ns->len = strlen(ns->sep);
#else
				const int err = _yIO_strconv_str_to_πstr(sep, strlen(sep), &ns->sep, &ns->len);
				if (err) return err;
#endif
			}
#else
			ns->sep = DEFAULT_THOUSEND_SEP;
			ns->len = 1;
#endif
		} else {
			ns->sep = &t->pf.grouping;
			ns->len = 1;
		}
	}
	if (ns->len == 0) return 0;
	return yπio_printctx_raw_write(t, ns->sep, ns->len);
}

static inline
void print_numsep_end(yπio_printctx_t *t, struct numsep *ns) {
#if TMODEX != 1
	if (ns->sep != &t->pf.grouping && ns->sep != DEFAULT_THOUSEND_SEP) {
		free((void *)ns->sep);
	}
#endif
}

static inline
int print_dot(yπio_printctx_t *t) {
#if YIO_USE_LOCALE
	if (t->pf.grouping == TC('L')) {
		const char *dot = nl_langinfo(RADIXCHAR); // DECIMAL_POINT
		if (dot == NULL) return 0;
		const TCHAR *tmpstr = NULL;
		size_t tmpstrlen = 0;
		int err = _yIO_strconv_str_to_πstr(dot, strlen(dot), &tmpstr, &tmpstrlen);
		if (err) return err;
		err = yπio_printctx_raw_write(t, tmpstr, tmpstrlen);
		_yIO_strconv_free_str_to_πstr(dot, tmpstr);
		return err;
	}
#endif
	const TCHAR DEFAULT_DOT[1] = { TC('.') };
	return yπio_printctx_raw_write(t, DEFAULT_DOT, 1);
}

static inline
int _yΩIO_print_format_generic_number_grouping(yπio_printctx_t *t, const TCHAR str[], size_t str_len) {
	const TCHAR *num = str;
	const TCHAR *const dotornul = str_dot_or_end(str, str_len);
	size_t numlen = dotornul - str;
	const char *group = get_group(t);
	const char *gri = group;
	unsigned groupsum = 0;
	char c;
	int err = 0;
	struct numsep ns = { NULL };
	for (; (c = *gri) != '\0' && c != CHAR_MAX; ++gri) {
		groupsum += c;
	}
	if (numlen > groupsum) {
		unsigned before = numlen - groupsum;
		if (c == '\0') {
			// get_group guarantees that we have at least 2 bytes
			assert(gri != group);
			const char lastgroup = *(gri - 1);
			const unsigned odd = before % lastgroup;
			if (odd) {
				err = yπio_printctx_raw_write(t, num, odd);
				if (err) goto NUMSEP_END;
				num += odd;
				before -= odd;
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
			}
			for (const TCHAR *const endbefore = num + before; num != endbefore; num += lastgroup) {
				err = yπio_printctx_raw_write(t, num, lastgroup);
				if (err) goto NUMSEP_END;
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
			}
		} else {
			assert(c == CHAR_MAX);
			err = yπio_printctx_raw_write(t, num, before);
			if (err) goto NUMSEP_END;
			num += before;
			if (groupsum) {
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
			}
		}
		numlen = groupsum;
	}
	while (groupsum) {
		groupsum -= *--gri;
		if (numlen > groupsum) {
			const unsigned left = numlen - groupsum;
			err = yπio_printctx_raw_write(t, num, left);
			if (err) goto NUMSEP_END;
			while (groupsum) {
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
				c = *--gri;
				groupsum -= c;
				err = yπio_printctx_raw_write(t, num, c);
				if (err) goto NUMSEP_END;
			}
			break;
		}
	}
	assert(gri == group);
	assert(num <= str + str_len);
	print_numsep_end(t, &ns);
	//
	if (num != str + str_len) {
		err = print_dot(t);
		if (err) return err;
		if (str_len > 1) {
			err = yπio_printctx_raw_write(t, num, str_len - 1);
			if (err) return err;
		}
	}
	// SUCCESS
	return err;
NUMSEP_END:
	print_numsep_end(t, &ns);
	return err;
}

static inline
int _yΩIO_printformat_print(_yΩIO_printformat_t *pf, const TCHAR str[], size_t str_len) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	if (is_number == true && f->grouping != TC('\0')) {
		const int err = _yΩIO_print_format_generic_number_grouping(t, str, str_len);
		if (err) return err;
	} else {
		const int err = yπio_printctx_raw_write(t, str, str_len);
		if (err) return err;
	}
	return 0;
}

static inline
bool is_one_of_or_nul(TCHAR c, const TCHAR *str) {
	return c == TC('\0') || _yΩIO_strnulchrbool(str, c);
}

static inline
void _yΩIO_printformat_assert_valid(const struct yπio_printfmt_s *pf) {
	assert(is_one_of_or_nul(pf->align, TC("<>=^")));
	assert(is_one_of_or_nul(pf->sign, TC("+- ")));
	assert(pf->fill != TC('{') && pf->fill != TC('}'));
	assert(is_one_of_or_nul(pf->grouping, TC("_,L")));
}

int _yΩIO_printformat_generic(yπio_printctx_t * restrict t,
		const TCHAR str[restrict], size_t str_len, bool is_number, bool is_positive) {
	_yΩIO_printformat_assert_valid(&t->pf);
	_yΩIO_printformat_t pf;
	_yΩIO_printformat_init(&pf, t, str, str_len, is_number, is_positive);
	int err = _yΩIO_printformat_prefix(&pf);
	if (err) return err;
	err = _yΩIO_printformat_print(&pf, str, str_len);
	if (err) return err;
	err = _yΩIO_printformat_suffix(&pf);
	if (err) return err;
	return 0;
}

#if TMODEX != 1
int _yΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive) {
	const TCHAR *dest = NULL;
	size_t dest_len = 0;
	int ret = _yIO_strconv_str_to_πstr(str, str_len, &dest, &dest_len);
	if (ret) return ret;
	ret = _yΩIO_printformat_generic(t, dest, dest_len, is_number, is_positive);
	_yIO_strconv_free_str_to_πstr(str, dest);
	return ret;
}
#endif
