/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "yio/yio_error.h"
#define _XOPEN_SOURCE  1 // wcswidth
#define _POSIX_C_SOURCE  200112L  // nl_langinfo
#define _GNU_SOURCE  1 // GROUPING (?)
#include "ctx.h"
#include "private.h"
#ifndef YYIO_HAS_UNISTRING
#error
#endif
#if YYIO_HAS_UNISTRING
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

static const TCHAR YYΩIO_ALIGN_LEFT = TC('<');
static const TCHAR YYΩIO_ALIGN_RIGHT = TC('>');
static const TCHAR YYΩIO_ALIGN_PADSIGN = TC('=');
static const TCHAR YYΩIO_ALIGN_CENTER = TC('^');

static const TCHAR YYΩIO_SIGN_ALWAYS = TC('+');
//static const TCHAR YYΩIO_SIGN_NEGATIVE = TC('-');
static const TCHAR YYΩIO_SIGN_ALWAYSSPACE = TC(' ');

const struct yπio_printfmt_s YYΩIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = TC(' '),
		.sign = TC('-'),
};

/* ------------------------------------------------------------------------- */

void YYΩIO_skip_arm(yπio_printctx_t *t, unsigned count) {
	va_end(*t->va);
	va_copy(*t->va, *t->startva);
	t->ifunc = t->startifunc;
	t->skip = count;
}

int YYΩIO_skip_do(yπio_printctx_t *t) {
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
int YYΩIO_digit_to_number(TCHAR d) {
	assert(TISDIGIT(d));
#if TMODEX == 1
	return d - '0';
#else
	const TCHAR table[] = TC("0123456789");
	return (int)(TSTRCHR(table, d) - table);
#endif
}

int YYΩIO_printctx_strtoi_noerr(const TCHAR **fmtpnt) {
	const TCHAR *fmt = *fmtpnt;
	assert(TISDIGIT(fmt[0]));
	int num = 0;
	do {
		assert(num < INT_MAX / 10);
		num *= 10;
		const int c = YYΩIO_digit_to_number(fmt[0]);
		assert(num < INT_MAX - c);
		num += c;
		++fmt;
	} while (TISDIGIT(fmt[0]));
	*fmtpnt = fmt;
	return num;
}

static inline
int YYΩIO_printctx_take_positional_param(yπio_printctx_t *t, const TCHAR *fmt, const TCHAR **endptr, int *res) {
	assert(fmt[0] == TC('{'));
	fmt++;
	if (TISDIGIT(fmt[0])) {
		YYΩIO_skip_arm(t, YYΩIO_printctx_strtoi_noerr(&fmt));
		const int skiperr = YYΩIO_skip_do(t);
		if (skiperr) return skiperr;
	}
	if (t->ifunc == NULL) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NO_ARGS, "no arguments for positional width or precision");
	}
	int num;
	const yπio_printdata_t ifunc = *t->ifunc++;
	// TODO: conversions
	if (ifunc == &YYΩIO_print_short)       num = yπio_printctx_va_arg_promote(t, short);
	else if (ifunc == &YYΩIO_print_ushort) num = yπio_printctx_va_arg_promote(t, unsigned short);
	else if (ifunc == &YYΩIO_print_int)    num = yπio_printctx_va_arg(t, int);
	else if (ifunc == &YYΩIO_print_uint)   num = yπio_printctx_va_arg(t, unsigned int); // NOLINT
	else if (ifunc == &YYΩIO_print_long)   num = yπio_printctx_va_arg(t, long);
	else if (ifunc == &YYΩIO_print_ulong)  num = yπio_printctx_va_arg(t, unsigned long);
	else if (ifunc == &YYΩIO_print_llong)  num = yπio_printctx_va_arg(t, long long);
	else if (ifunc == &YYΩIO_print_ullong) num = yπio_printctx_va_arg(t, unsigned long long);
	else return YYIO_ERROR(YIO_ERROR_POSITIONAL_NOT_NUMBER, "positional width or precision specifier is not a number");
	if (fmt++[0] != TC('}')) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_MISSING_RIGHT_BRACE, "missing '}' when parsing positional width or precision specifier");
	}
	if (num < 0) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NEGATIVE, "positional width or precision specifier cannot be negative");
	}
	*endptr = fmt;
	*res = num;
	return 0;
}

int YYΩIO_printctx_stdintparam(yπio_printctx_t *t, const TCHAR *fmt, const TCHAR **endptr, int *res) {
	if (fmt[0] == TC('{')) {
		const int ret = YYΩIO_printctx_take_positional_param(t, fmt, endptr, res);
		if (ret) return ret;
	} else if (TISDIGIT(fmt[0])) {
		*res = YYΩIO_printctx_strtoi_noerr(&fmt);
		*endptr = fmt;
	} else {
		// do nothing
		*endptr = fmt;
	}
	return 0;
}

bool YYΩIO_strnulchrbool(const TCHAR *s, TCHAR c) {
	return c != TC('\0') && TSTRCHR(s, c) != NULL;
}

int YYΩIO_pfmt_parse(struct YYΩIO_printctx_s *t, struct yπio_printfmt_s *pf,
		const TCHAR *fmt, const TCHAR **endptr) {
	/*
	https://fmt.dev/latest/syntax.html#format-specification-mini-language
	format_spec     ::=  [[fill]align][sign][#][0][width][grouping_option][.precision]["L"][type]
	fill            ::=  <any character except { or }>
	align           ::=  "<" | ">" | "=" | "^"
	sign            ::=  "+" | "-" | " "
	width           ::=  digit+
	grouping_option ::=  "_" | ","
	precision       ::=  digit+
	type            ::=  "b" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | "o" | "s" | "x" | "X" | "%"
	 */

	// fill and align must be first
	if (fmt[0] != TC('\0') && YYΩIO_strnulchrbool(TC("<>=^"), fmt[1])) {
		pf->fill = fmt++[0];
		pf->align = fmt++[0];
	} else if (YYΩIO_strnulchrbool(TC("<>=^"), fmt[0])) {
		pf->align = fmt++[0];
	}

	int ret = 0;
	// I am parsing in a loop, but still it is undefined behavior to specify them out-of-place.
	while (1) {
		const TCHAR ch = fmt++[0];
		switch (ch) {
		case TC('}'):
			goto EXIT;
		case TC('\0'):
			ret = YYIO_ERROR(YIO_ERROR_MISSING_RIGHT_BRACE, "missing '}' when parsing common format specification");
			goto EXIT;
		case TC('+'):
		case TC('-'):
		case TC(' '):
			pf->sign = ch;
			break;
		case TC('#'):
			pf->hash = true;
			break;
		case TC('0'):
			if (pf->align == TC('\0')) {
				pf->align = TC('0');
			}
			break;
		case TC('1'):
		case TC('2'):
		case TC('3'):
		case TC('4'):
		case TC('5'):
		case TC('6'):
		case TC('7'):
		case TC('8'):
		case TC('9'):
		case TC('{'):
			--fmt;
			ret = YYΩIO_printctx_stdintparam(t, fmt, &fmt, &pf->width);
			if (ret) goto EXIT;
			break;
		case TC('_'):
		case TC(','):
		case TC('L'):
			pf->grouping = ch;
			break;
		case TC('.'):
			{
				const TCHAR *endparamptr;
				ret = YYΩIO_printctx_stdintparam(t, fmt, &endparamptr, &pf->precision);
				if (ret) goto EXIT;
				// If there is a dot, there must be precision.
				if (endparamptr == fmt) {
					ret = YYIO_ERROR(YIO_ERROR_MISSING_PRECISION, "Format specifier missing precision");
					goto EXIT;
				}
				fmt = endparamptr;
			}
			break;
		case TC('c'):
		case TC('s'):
		case TC('d'):
		case TC('e'): case TC('E'):
		case TC('f'): case TC('F'):
		case TC('a'): case TC('A'):
		case TC('g'): case TC('G'):
		case TC('b'): case TC('B'):
		case TC('o'): case TC('O'):
		case TC('x'): case TC('X'):
		case TC('n'):
		case TC('p'):
			pf->type = ch;
			break;
		default:
			ret = YYIO_ERROR(YIO_ERROR_FMT_UNKNOWN, "Invalid character in standard format specification");
			goto EXIT;
		}
	}
	// SUCCESS
	assert(fmt[-1] == '}');
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
		const int err = YYΩIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt);
		if (err) return err;
	}
	return 0;
}

int yπio_printctx_raw_write(yπio_printctx_t *t, const TCHAR *restrict ptr, size_t size) {
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = (*t->out)(t->outarg, ptr, size);
	if (ret) return ret;
	//dbgln("%d", (int)size);
	t->writtencnt += size;
	return 0;
}

int yπio_printctx_next(yπio_printctx_t *t) {
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	if (*t->ifunc == NULL) {
		return YYIO_ERROR(YIO_ERROR_NO_NEXT, "formatting modifier is not followed by an argument");
	}
	return (*t->ifunc)(t);
}

int YYΩIO_printctx_print_in(yπio_printctx_t *t, yπio_printdata_t *data, const TCHAR *fmt, ...) {
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

#ifndef YYIO_HAS_wcswidth
#error
#endif
#ifndef YYIO_HAS_UNISTRING
#error
#endif
static inline
size_t YYΩIO_width(const TCHAR *str, size_t str_len) {
#if TMODE == 1 && YYIO_HAS_UNISTRING
	return u8_width((const uint8_t*)str, str_len, locale_charset());
#elif TMODE == 2 && YYIO_HAS_wcswidth
	const int width = wcswidth(str, str_len);
	return width < 0 ? str_len : (size_t)width;
#elif TMODE == 3
	return u16_width(str, str_len, locale_charset());
#elif TMODE == 4
	return u32_width(str, str_len, locale_charset());
#else
	return str_len;
#endif
}

typedef struct YYΩIO_printformat_t {
	yπio_printctx_t *t;
	size_t str_len;
	size_t alllen;
	bool is_number;
	bool is_positive;
} YYΩIO_printformat_t;

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
void YYΩIO_printformat_init(YYΩIO_printformat_t *pf, yπio_printctx_t *t,
		const TCHAR *str, size_t str_len, bool is_number, bool is_positive) {
	YYΩIO_printformat_t ret = {
			.t = t,
			.str_len = (is_number ? str_len : YYΩIO_width(str, str_len)),
			.is_number = is_number,
			.is_positive = is_positive,
	};
	*pf = ret;
}

static inline
int YYΩIO_printformat_prefix__print_sign_hash(yπio_printctx_t *t,
		const struct yπio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const TCHAR c = (TCHAR)(is_positive ? f->sign : TC('-'));
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
int YYΩIO_printformat_prefix(YYΩIO_printformat_t *pf) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const bool is_positive = pf->is_positive;
	size_t * const alllen0 = &pf->alllen;
	const size_t len = pf->str_len;

	const bool has_hash = is_number && f->hash && TSTRCHR(TC("xXoObB"), f->type) != NULL;
	const bool has_sign = is_number && (f->sign == YYΩIO_SIGN_ALWAYS ||
					f->sign == YYΩIO_SIGN_ALWAYSSPACE || is_positive == false);
	const size_t alllen = len + (size_t)( 2U * has_hash + has_sign );
	*alllen0 = alllen;
	const size_t width = f->width > 0 ? f->width : 0;

	if (f->align == TC('\0')) {
		// The default for numbers is right, otherwise it's left.
		f->align = pf->is_number ? YYΩIO_ALIGN_RIGHT : YYΩIO_ALIGN_LEFT;
	}

	if (f->align == YYΩIO_ALIGN_PADSIGN) {
		const int err = YYΩIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	if ((f->align == YYΩIO_ALIGN_PADSIGN ||
			f->align == YYΩIO_ALIGN_RIGHT ||
			f->align == YYΩIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = width - alllen;
		size_t diff = f->align == YYΩIO_ALIGN_CENTER ? tmp / 2 : tmp;
		while (diff--) {
			const int err = yπio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}

	if (f->align != YYΩIO_ALIGN_PADSIGN) {
		const int err = YYΩIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	return 0;
}

static inline
int YYΩIO_printformat_suffix(YYΩIO_printformat_t *pf) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const size_t alllen = pf->alllen;
	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == YYΩIO_ALIGN_LEFT || f->align == YYΩIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == YYΩIO_ALIGN_CENTER ? tmp / 2 + !!(tmp % 2) : tmp;
		while (diff--) {
			const int err = yπio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}
	return 0;
}

static inline
const TCHAR *str_dot_or_end(const TCHAR str[], size_t len) {
	for (; len != 0 && str[0] != TC('.') && str[0] != TC(','); ++str, --len) {
	}
	return str;
}

#if YIO_USE_LOCALE
static const char NOGROUP[1] = { CHAR_MAX };
#endif
static const char GROUP3[2] = "\x03";
static const char GROUP4[2] = "\x04";

static inline
const char *get_group(yπio_printctx_t *t) {
#if YIO_USE_LOCALE && defined(GROUPING)
	if (t->pf.grouping == TC('L')) {
		const char *r = nl_langinfo(GROUPING);
		return r != NULL && *r != '\0' ? r : NOGROUP;
	}
#endif
	//dbgln("HERE %c", (int)t->pf.type);
	return YYΩIO_strnulchrbool(TC("bBxX"), t->pf.type) ? GROUP4 : GROUP3;
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
				// No separator, set sep to non-null to pass check above.
				ns->sep = DEFAULT_THOUSEND_SEP;
				ns->len = 0;
			} else {
#if TMODEX == 1
				ns->sep = sep;
				ns->len = strlen(ns->sep);
#else
				// Convert separator to wchar/char16/char32.
				const int err = YYIO_strconv_str_to_πstr(sep, strlen(sep), &ns->sep, &ns->len);
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
		free((void *)ns->sep); // cppcheck-suppress cert-EXP05-C
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
		int err = YYIO_strconv_str_to_πstr(dot, strlen(dot), &tmpstr, &tmpstrlen);
		if (err) return err;
		err = yπio_printctx_raw_write(t, tmpstr, tmpstrlen);
		YYIO_strconv_free_str_to_πstr(dot, tmpstr);
		return err;
	}
#endif
	const TCHAR DEFAULT_DOT[1] = { TC('.') };
	return yπio_printctx_raw_write(t, DEFAULT_DOT, 1);
}

static inline
int YYΩIO_print_format_generic_number_grouping(yπio_printctx_t *t, const TCHAR str[], size_t str_len) {
	const TCHAR *num = str;
	const TCHAR *const dotorend = str_dot_or_end(str, str_len);
	size_t numlen = dotorend - str;
	const char *group = get_group(t);
	const char *gri = group;
	unsigned groupsum = 0;
	char c;
	int err = 0;
	struct numsep ns = { NULL, 0 };
	for (; (c = *gri) != '\0' && c != CHAR_MAX; ++gri) {
		groupsum += c;
	}
	if (numlen > groupsum) {
		unsigned before = numlen - groupsum;
		if (c == '\0') {
			// get_group guarantees that we have at least 2 bytes
			assert(gri != group);
			const char lastgroup = gri[-1];
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
			num += left;
			while (groupsum) {
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
				c = *--gri;
				groupsum -= c;
				err = yπio_printctx_raw_write(t, num, c);
				if (err) goto NUMSEP_END;
				num += c;
			}
			break;
		}
	}
	assert(gri == group);
	assert(num <= str + str_len);
	//dbgln("%s %s", num, dotorend);
	assert(num == dotorend);
	print_numsep_end(t, &ns);
	//
	const size_t postdotlen = str_len - (num - str);
	if (postdotlen) {
		// We are at dot
		assert(*num == TC('.') || *num == TC(','));
		err = print_dot(t);
		if (err) return err;
		if (postdotlen > 1) {
			// We are after dot
			err = yπio_printctx_raw_write(t, num + 1, postdotlen - 1);
			if (err) return err;
		}
		num += postdotlen;
	}
	assert(str + str_len == num);
	// SUCCESS
	return err;
NUMSEP_END:
	print_numsep_end(t, &ns);
	return err;
}

static inline
int YYΩIO_printformat_print(YYΩIO_printformat_t *pf, const TCHAR str[], size_t str_len) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	if (is_number == true && f->grouping != TC('\0')) {
		const int err = YYΩIO_print_format_generic_number_grouping(t, str, str_len);
		if (err) return err;
	} else {
		const int err = yπio_printctx_raw_write(t, str, str_len);
		if (err) return err;
	}
	return 0;
}

static inline
bool is_one_of_or_nul(TCHAR c, const TCHAR *str) {
	return c == TC('\0') || YYΩIO_strnulchrbool(str, c);
}

static inline
void YYΩIO_printformat_assert_valid(const struct yπio_printfmt_s *pf) {
	assert(is_one_of_or_nul(pf->align, TC("<>=^")));
	assert(is_one_of_or_nul(pf->sign, TC("+- ")));
	assert(pf->fill != TC('{') && pf->fill != TC('}'));
	assert(is_one_of_or_nul(pf->grouping, TC("_,L")));
	assert(is_one_of_or_nul(pf->c_onversion, TC("a")));
}

/* ------------------------------------------------------------------------- */

static inline
bool is_print_ascii(TCHAR tcc) {
	const unsigned char ascii_min_printable = 32U;
	const unsigned char ascii_max_printable = 126U;
#if TMODE == 1 || \
		(TMODE == 2 && !defined(__STDC_MB_MIGHT_NEQ_WC__)) || \
		(TMODE == 3 && defined(__STDC_UTF_16__)) || \
		(TMODE == 4 && defined(__STDC_UTF_32__))
#else
#warning TODO: conversion
#endif
	const TCHAR cc = tcc;
	return ascii_min_printable <= cc && cc <= ascii_max_printable;
}

union out_or_counting_u {
	TCHAR *newstr;
	size_t cnt;
};

struct ss_s {
	union out_or_counting_u uu;
	bool counting;
};

static inline
struct ss_s ss_init(TCHAR *newstr) {
	struct ss_s rr;
	if (newstr != NULL) {
		rr.counting = false;
		rr.uu.newstr = newstr;
	} else {
		rr.counting = true;
		rr.uu.cnt = 0;
	}
	return rr;
}

static inline
void ss_out(struct ss_s *t, TCHAR cc) {
	if (t->counting) {
		t->uu.cnt++;
	} else {
		if (t->uu.newstr == NULL) __builtin_unreachable();
		*t->uu.newstr++ = cc;
	}
}

static const TCHAR *const xdigits = TC("0123456789abcdef");
static const unsigned char four = 0xfU;

static inline
void ascii_encode_x(struct ss_s *ss, TCHAR cc, TCHAR next) {
	static_assert(CHAR_BIT == 8, "Really? TODO");
	const bool nextdigit = !!TISXDIGIT(next);
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		ss_out(ss, TC('\\'));
		ss_out(ss, TC('x'));
		if (nextdigit || bbv > (TCHAR)16U) {
			ss_out(ss, xdigits[(bbv >> 4U) & four]);
		}
		ss_out(ss, xdigits[bbv & four]);
	}
}

static inline
void ascii_encode_o(struct ss_s *ss, TCHAR cc, TCHAR next) {
	static const unsigned char three = 0x7U;
	const bool nextdigit = !!TISDIGIT(next);
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		ss_out(ss, TC('\\'));
		for (unsigned ii = 3U * 2U; ii > 0U; ii -= 3U) {
			if (nextdigit || bbv > (TCHAR)(1U << ii)) {
				ss_out(ss, xdigits[(bbv >> ii) & three]);
			}
		}
		ss_out(ss, xdigits[bbv & three]);
	}
}

static inline
void ascii_encode_u(struct ss_s *ss, TCHAR cc, TCHAR next) {
	const bool nextdigit = !!TISXDIGIT(next);
	ss_out(ss, TC('\\'));
	ss_out(ss, TC('u'));
	for (unsigned ii = 4U * 3U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (TCHAR)(1U << ii)) {
			ss_out(ss, xdigits[(cc >> ii) & four]);
		}
	}
	ss_out(ss, xdigits[cc & four]);
}

static inline
void ascii_encode_U(struct ss_s *ss, TCHAR cc, TCHAR next) {
	const bool nextdigit = !!TISXDIGIT(next);
	ss_out(ss, TC('\\'));
	ss_out(ss, TC('U'));
	for (unsigned ii = 4U * 7U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (TCHAR)(1U << ii)) {
			ss_out(ss, xdigits[(cc >> ii) & four]);
		}
	}
	ss_out(ss, xdigits[cc & four]);
}

static inline
TCHAR ascii_encode_get_esc(TCHAR prev, TCHAR cc) {
	switch (cc) {
	case TC('\''): return TC('\'');
	case TC('\"'): return TC('\"');
	case TC('\a'): return TC('t');
	case TC('\b'): return TC('b');
	case TC('\f'): return TC('f');
	case TC('\n'): return TC('n');
	case TC('\r'): return TC('r');
	case TC('\t'): return TC('t');
	case TC('\v'): return TC('v');
	case TC('\?'):
		// If the previous character was ?, then watch out for trigraphs.
		if (prev == '?') {
			return TC('?');
		}
		break;
	}
	return TC('\0');
}

#define CSTRLEN(x)  (sizeof(x) - 1)

#if (TMODE == 2 && defined(__STDC_ISO_10646__) && WCHAR_MAX == INT16_MAX) || (TMODE == 3 && defined(__STDC_UTF_16__))
#define ASCII_ENCODE  ascii_encode_u  // \u1234
#elif (TMODE == 2 && defined(__STDC_ISO_10646__) && WCHAR_MAX == INT32_MAX) || (TMODE == 4 && defined(__STDC_UTF_32__))
#define ASCII_ENCODE  ascii_encode_U  // \U12345678
#else
#define ASCII_ENCODE  ascii_encode_o  // \1\123\377...
#endif

static inline
struct ss_s ascii_encode_do(const TCHAR str[restrict], size_t str_len, TCHAR *restrict newstr, size_t newstr_len) {
	struct ss_s ss_mem = ss_init(newstr);
	struct ss_s *ss = &ss_mem;
	for (TCHAR prev = TC('\0'), next = *str, cc = next;
			cc != TC('\0');
			prev = cc, cc = next) {
		next = *(++str);
		//
		const TCHAR esc = ascii_encode_get_esc(prev, cc);
		if (esc != TC('\0')) {
			//dbgln("%c  %#x adding\\%c", cc, cc, esc);
			ss_out(ss, TC('\\'));
			ss_out(ss, esc);
		} else if (is_print_ascii(cc)) {
			//dbgln("%c  %#x is_ascii", cc, cc);
			ss_out(ss, cc);
		} else {
			ASCII_ENCODE(ss, cc, next);
		}
	}
	if (newstr != NULL) {
		(void)newstr_len;
		assert(ss->uu.newstr == newstr + newstr_len);
	}
	return ss_mem;
}

static inline
size_t ascii_encode_get_length(const TCHAR str[restrict], size_t str_len) {
	return ascii_encode_do(str, str_len, NULL, 0).uu.cnt;
}

static inline
int YYΩIO_printformat_conversion(yπio_printctx_t *restrict t,
		const TCHAR *restrict *restrict pstr, size_t *pstr_len) {
	//dbgln("%c", t->pf.c_onversion);
	if (t->pf.c_onversion != TC('a')) return 0;
	const TCHAR *restrict str = *pstr;
	const size_t str_len = *pstr_len;
	const size_t newstr_len = ascii_encode_get_length(str, str_len);
	if (newstr_len == str_len) return 0;
	assert(newstr_len > str_len);
	TCHAR *const newstr = malloc(newstr_len * sizeof(*newstr));
	if (newstr == NULL) return YIO_ERROR_ENOMEM;
	(void)ascii_encode_do(str, str_len, newstr, newstr_len);
	*pstr_len = newstr_len;
	*pstr = newstr;
	return 1;
}

/* ------------------------------------------------------------------------- */

int YYΩIO_printformat_generic(yπio_printctx_t *restrict t,
		const TCHAR str[restrict], size_t str_len, bool is_number, bool is_positive) {
	int err = 0;
	const int converted = YYΩIO_printformat_conversion(t, &str, &str_len);
	if (converted < 0) return converted;
	//
	YYΩIO_printformat_assert_valid(&t->pf);
	YYΩIO_printformat_t pf;
	YYΩIO_printformat_init(&pf, t, str, str_len, is_number, is_positive);
	err = YYΩIO_printformat_prefix(&pf);
	if (err) goto EXIT;
	err = YYΩIO_printformat_print(&pf, str, str_len);
	if (err) goto EXIT;
	err = YYΩIO_printformat_suffix(&pf);
	if (err) goto EXIT;
	//
EXIT:
	if (converted) {
		free((void *)str);
	}
	return err;
}

#if TMODE != 1
int YYΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive) {
	const TCHAR *dest = NULL;
	size_t dest_len = 0;
	int ret = YYIO_strconv_str_to_πstr(str, str_len, &dest, &dest_len);
	if (ret) return ret;
	ret = YYΩIO_printformat_generic(t, dest, dest_len, is_number, is_positive);
	YYIO_strconv_free_str_to_πstr(str, dest);
	return ret;
}
#endif
