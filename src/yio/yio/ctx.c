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

static const char YYIO_ALIGN_LEFT = '<';
static const char YYIO_ALIGN_RIGHT = '>';
static const char YYIO_ALIGN_PADSIGN = '=';
static const char YYIO_ALIGN_CENTER = '^';

static const char YYIO_SIGN_ALWAYS = '+';
//static const char YYIO_SIGN_NEGATIVE = '-';
static const char YYIO_SIGN_ALWAYSSPACE = ' ';

const struct yio_printfmt_s YYIO_printfmt_default = {
		.width = -1,
		.precision = -1,
		.fill = ' ',
		.sign = '-',
};

/* ------------------------------------------------------------------------- */

void YYIO_skip_arm(yio_printctx_t *t, unsigned count) {
	va_end(*t->va);
	va_copy(*t->va, *t->startva);
	t->ifunc = t->startifunc;
	t->skip = count;
}

int YYIO_skip_do(yio_printctx_t *t) {
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
int YYIO_digit_to_number(char d) {
	assert(isdigit((unsigned char)(d)));
	return d - '0';
}

int YYIO_printctx_strtoi_noerr(const char **fmtpnt) {
	const char *fmt = *fmtpnt;
	assert(isdigit((unsigned char)(fmt[0])));
	int num = 0;
	do {
		assert(num < INT_MAX / 10);
		num *= 10;
		const int c = YYIO_digit_to_number(fmt[0]);
		assert(num < INT_MAX - c);
		num += c;
		++fmt;
	} while (isdigit((unsigned char)(fmt[0])));
	*fmtpnt = fmt;
	return num;
}

static inline
int YYIO_printctx_take_positional_param(yio_printctx_t *t, const char *fmt, const char **endptr, int *res) {
	assert(fmt[0] == '{');
	fmt++;
	if (isdigit((unsigned char)(fmt[0]))) {
		YYIO_skip_arm(t, YYIO_printctx_strtoi_noerr(&fmt));
		const int skiperr = YYIO_skip_do(t);
		if (skiperr) return skiperr;
	}
	if (t->ifunc == NULL) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NO_ARGS, "no arguments for positional width or precision");
	}
	int num;
	const yio_printdata_t ifunc = *t->ifunc++;
	// TODO: conversions
	if (ifunc == &YYIO_print_short)       num = yio_printctx_va_arg_promote(t, short);
	else if (ifunc == &YYIO_print_ushort) num = yio_printctx_va_arg_promote(t, unsigned short);
	else if (ifunc == &YYIO_print_int)    num = yio_printctx_va_arg(t, int);
	else if (ifunc == &YYIO_print_uint)   num = yio_printctx_va_arg(t, unsigned int); // NOLINT
	else if (ifunc == &YYIO_print_long)   num = yio_printctx_va_arg(t, long);
	else if (ifunc == &YYIO_print_ulong)  num = yio_printctx_va_arg(t, unsigned long);
	else if (ifunc == &YYIO_print_llong)  num = yio_printctx_va_arg(t, long long);
	else if (ifunc == &YYIO_print_ullong) num = yio_printctx_va_arg(t, unsigned long long);
	else return YYIO_ERROR(YIO_ERROR_POSITIONAL_NOT_NUMBER, "positional width or precision specifier is not a number");
	if (fmt++[0] != '}') {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_MISSING_RIGHT_BRACE, "missing '}' when parsing positional width or precision specifier");
	}
	if (num < 0) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NEGATIVE, "positional width or precision specifier cannot be negative");
	}
	*endptr = fmt;
	*res = num;
	return 0;
}

int YYIO_printctx_stdintparam(yio_printctx_t *t, const char *fmt, const char **endptr, int *res) {
	if (fmt[0] == '{') {
		const int ret = YYIO_printctx_take_positional_param(t, fmt, endptr, res);
		if (ret) return ret;
	} else if (isdigit((unsigned char)(fmt[0]))) {
		*res = YYIO_printctx_strtoi_noerr(&fmt);
		*endptr = fmt;
	} else {
		// do nothing
		*endptr = fmt;
	}
	return 0;
}

bool YYIO_strnulchrbool(const char *s, char c) {
	return c != '\0' && strchr(s, c) != NULL;
}

int YYIO_pfmt_parse(struct YYIO_printctx_s *t, struct yio_printfmt_s *pf,
		const char *fmt, const char **endptr) {
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
	if (fmt[0] != '\0' && YYIO_strnulchrbool("<>=^", fmt[1])) {
		pf->fill = fmt++[0];
		pf->align = fmt++[0];
	} else if (YYIO_strnulchrbool("<>=^", fmt[0])) {
		pf->align = fmt++[0];
	}

	int ret = 0;
	// I am parsing in a loop, but still it is undefined behavior to specify them out-of-place.
	while (1) {
		const char ch = fmt++[0];
		switch (ch) {
		case '}':
			goto EXIT;
		case '\0':
			ret = YYIO_ERROR(YIO_ERROR_MISSING_RIGHT_BRACE, "missing '}' when parsing common format specification");
			goto EXIT;
		case '+':
		case '-':
		case ' ':
			pf->sign = ch;
			break;
		case '#':
			pf->hash = true;
			break;
		case '0':
			if (pf->align == '\0') {
				pf->fill = '0';
				pf->align = '=';
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '{':
			--fmt;
			ret = YYIO_printctx_stdintparam(t, fmt, &fmt, &pf->width);
			if (ret) goto EXIT;
			break;
		case '_':
		case ',':
		case 'L':
			pf->grouping = ch;
			break;
		case '.':
			{
				const char *endparamptr;
				ret = YYIO_printctx_stdintparam(t, fmt, &endparamptr, &pf->precision);
				if (ret) goto EXIT;
				// If there is a dot, there must be precision.
				if (endparamptr == fmt) {
					ret = YYIO_ERROR(YIO_ERROR_MISSING_PRECISION, "Format specifier missing precision");
					goto EXIT;
				}
				fmt = endparamptr;
			}
			break;
		case 'c':
		case 's':
		case 'd':
		case 'e': case 'E':
		case 'f': case 'F':
		case 'a': case 'A':
		case 'g': case 'G':
		case 'b': case 'B':
		case 'o': case 'O':
		case 'x': case 'X':
		case 'n':
		case 'p':
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

int yio_printctx_init(yio_printctx_t *t) {
	if (t->skip != 0) {
		return YIO_ERROR_SKIPPING;
	}
	if (t->fmt) {
		const int err = YYIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt);
		if (err) return err;
	}
	return 0;
}

int yio_printctx_raw_write(yio_printctx_t *t, const char *restrict ptr, size_t size) {
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = (*t->out)(t->outarg, ptr, size);
	if (ret) return ret;
	//dbgln("%d", (int)size);
	t->writtencnt += size;
	return 0;
}

int yio_printctx_next(yio_printctx_t *t) {
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	if (*t->ifunc == NULL) {
		return YYIO_ERROR(YIO_ERROR_NO_NEXT, "formatting modifier is not followed by an argument");
	}
	return (*t->ifunc)(t);
}

int YYIO_printctx_print_in(yio_printctx_t *t, yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yvbprintf(t->out, t->outarg, data, fmt, &va);
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
size_t YYIO_width(const char *str, size_t str_len) {
#if YYIO_HAS_UNISTRING
	return u8_width((const uint8_t*)str, str_len, locale_charset());
#else
	return str_len;
#endif
}

typedef struct YYIO_printformat_t {
	yio_printctx_t *t;
	size_t str_len;
	size_t alllen;
	bool is_number;
	bool is_positive;
} YYIO_printformat_t;

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
void YYIO_printformat_init(YYIO_printformat_t *pf, yio_printctx_t *t,
		const char *str, size_t str_len, bool is_number, bool is_positive) {
	YYIO_printformat_t ret = {
			.t = t,
			.str_len = (is_number ? str_len : YYIO_width(str, str_len)),
			.is_number = is_number,
			.is_positive = is_positive,
	};
	*pf = ret;
}

static inline
int YYIO_printformat_prefix__print_sign_hash(yio_printctx_t *t,
		const struct yio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const char c = (char)(is_positive ? f->sign : '-');
		const int err = yio_printctx_raw_write(t, &c, 1);
		if (err) return err;
	}
	if (has_hash) {
		const char buf[2] = { '0', f->type, };
		const int err = yio_printctx_raw_write(t, buf, 2);
		if (err) return err;
	}
	return 0;
}

static inline
int YYIO_printformat_prefix(YYIO_printformat_t *pf) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const bool is_positive = pf->is_positive;
	size_t * const alllen0 = &pf->alllen;
	const size_t len = pf->str_len;

	const bool has_hash = is_number && f->hash && strchr("xXoObB", f->type) != NULL;
	const bool has_sign = is_number && (f->sign == YYIO_SIGN_ALWAYS ||
					f->sign == YYIO_SIGN_ALWAYSSPACE || is_positive == false);
	const size_t alllen = len + (size_t)( 2U * has_hash + has_sign );
	*alllen0 = alllen;
	const size_t width = f->width > 0 ? f->width : 0;

	if (f->align == '\0') {
		// The default for numbers is right, otherwise it's left.
		f->align = pf->is_number ? YYIO_ALIGN_RIGHT : YYIO_ALIGN_LEFT;
	}

	if (f->align == YYIO_ALIGN_PADSIGN) {
		const int err = YYIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	if ((f->align == YYIO_ALIGN_PADSIGN ||
			f->align == YYIO_ALIGN_RIGHT ||
			f->align == YYIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = width - alllen;
		size_t diff = f->align == YYIO_ALIGN_CENTER ? tmp / 2 : tmp;
		while (diff--) {
			const int err = yio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}

	if (f->align != YYIO_ALIGN_PADSIGN) {
		const int err = YYIO_printformat_prefix__print_sign_hash(t, f,
				has_sign, has_hash, is_positive);
		if (err) return err;
	}

	return 0;
}

static inline
int YYIO_printformat_suffix(YYIO_printformat_t *pf) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const size_t alllen = pf->alllen;
	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == YYIO_ALIGN_LEFT || f->align == YYIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == YYIO_ALIGN_CENTER ? tmp / 2 + !!(tmp % 2) : tmp;
		while (diff--) {
			const int err = yio_printctx_raw_write(t, &f->fill, 1);
			if (err) return err;
		}
	}
	return 0;
}

static inline
const char *str_dot_or_end(const char str[], size_t len) {
	for (; len != 0 && str[0] != '.' && str[0] != ','; ++str, --len) {
	}
	return str;
}

#if YIO_USE_LOCALE
static const char NOGROUP[1] = { CHAR_MAX };
#endif
static const char GROUP3[2] = "\x03";
static const char GROUP4[2] = "\x04";

static inline
const char *get_group(yio_printctx_t *t) {
#if YIO_USE_LOCALE && defined(GROUPING)
	if (t->pf.grouping == 'L') {
		const char *r = nl_langinfo(GROUPING);
		return r != NULL && *r != '\0' ? r : NOGROUP;
	}
#endif
	//dbgln("HERE %c", (int)t->pf.type);
	return YYIO_strnulchrbool("bBxX", t->pf.type) ? GROUP4 : GROUP3;
}

struct numsep {
	const char *sep;
	size_t len;
};

static const char DEFAULT_THOUSEND_SEP[1] = { ',' };

static inline
int print_numsep(yio_printctx_t *t, struct numsep *ns) {
	if (ns->sep == NULL) {
		if (t->pf.grouping == 'L') {
#if YIO_USE_LOCALE
			const char *sep = nl_langinfo(THOUSEP); // THOUSEND_SEP
			if (sep == NULL) {
				// No separator, set sep to non-null to pass check above.
				ns->sep = DEFAULT_THOUSEND_SEP;
				ns->len = 0;
			} else {
				ns->sep = sep;
				ns->len = strlen(ns->sep);
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
	return yio_printctx_raw_write(t, ns->sep, ns->len);
}

static inline
void print_numsep_end(yio_printctx_t *t, struct numsep *ns) {
	(void)t;
	(void)ns;
}

static inline
int print_dot(yio_printctx_t *t) {
#if YIO_USE_LOCALE
	if (t->pf.grouping == 'L') {
		const char *dot = nl_langinfo(RADIXCHAR); // DECIMAL_POINT
		if (dot == NULL) return 0;
		const char *tmpstr = NULL;
		size_t tmpstrlen = 0;
		int err = YYIO_strconv_str_to_str(dot, strlen(dot), &tmpstr, &tmpstrlen);
		if (err) return err;
		err = yio_printctx_raw_write(t, tmpstr, tmpstrlen);
		YYIO_strconv_free_str_to_str(dot, tmpstr);
		return err;
	}
#endif
	const char DEFAULT_DOT[1] = { '.' };
	return yio_printctx_raw_write(t, DEFAULT_DOT, 1);
}

static inline
int YYIO_print_format_generic_number_grouping(yio_printctx_t *t, const char str[], size_t str_len) {
	const char *num = str;
	const char *const dotorend = str_dot_or_end(str, str_len);
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
				err = yio_printctx_raw_write(t, num, odd);
				if (err) goto NUMSEP_END;
				num += odd;
				before -= odd;
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
			}
			for (const char *const endbefore = num + before; num != endbefore; num += lastgroup) {
				err = yio_printctx_raw_write(t, num, lastgroup);
				if (err) goto NUMSEP_END;
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
			}
		} else {
			assert(c == CHAR_MAX);
			err = yio_printctx_raw_write(t, num, before);
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
			err = yio_printctx_raw_write(t, num, left);
			if (err) goto NUMSEP_END;
			num += left;
			while (groupsum) {
				err = print_numsep(t, &ns);
				if (err) goto NUMSEP_END;
				c = *--gri;
				groupsum -= c;
				err = yio_printctx_raw_write(t, num, c);
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
		assert(*num == '.' || *num == ',');
		err = print_dot(t);
		if (err) return err;
		if (postdotlen > 1) {
			// We are after dot
			err = yio_printctx_raw_write(t, num + 1, postdotlen - 1);
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
int YYIO_printformat_print(YYIO_printformat_t *pf, const char str[], size_t str_len) {
	yio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	if (is_number == true && f->grouping != '\0') {
		const int err = YYIO_print_format_generic_number_grouping(t, str, str_len);
		if (err) return err;
	} else {
		const int err = yio_printctx_raw_write(t, str, str_len);
		if (err) return err;
	}
	return 0;
}

static inline
bool is_one_of_or_nul(char c, const char *str) {
	return c == '\0' || YYIO_strnulchrbool(str, c);
}

static inline
void YYIO_printformat_assert_valid(const struct yio_printfmt_s *pf) {
	assert(is_one_of_or_nul(pf->align, "<>=^"));
	assert(is_one_of_or_nul(pf->sign, "+- "));
	assert(pf->fill != '{' && pf->fill != '}');
	assert(is_one_of_or_nul(pf->grouping, "_,L"));
	assert(is_one_of_or_nul(pf->c_onversion, "a"));
}

/* ------------------------------------------------------------------------- */

static inline
bool is_print_ascii(char tcc) {
	const unsigned char ascii_min_printable = 32U;
	const unsigned char ascii_max_printable = 126U;
	const char cc = tcc;
	return ascii_min_printable <= cc && cc <= ascii_max_printable;
}

struct ss_s {
	char *newstr;
	size_t cnt;
};

static inline
struct ss_s ss_init(char *newstr) {
	struct ss_s rr = { .newstr = newstr, .cnt = 0 };
	return rr;
}

static inline
void ss_out(struct ss_s *t, char cc) {
	if (t->newstr) {
		*t->newstr++ = cc;
	} else {
		t->cnt++;
	}
}

static const char *const xdigits = "0123456789abcdef";
static const unsigned char four = 0xfU;

static inline
void ascii_encode_x(struct ss_s *ss, char cc, char next) {
	static_assert(CHAR_BIT == 8, "Really? TODO");
	const bool nextdigit = !!isxdigit((unsigned char)(next));
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		ss_out(ss, '\\');
		ss_out(ss, 'x');
		if (nextdigit || bbv > (char)16U) {
			ss_out(ss, xdigits[(bbv >> 4U) & four]);
		}
		ss_out(ss, xdigits[bbv & four]);
	}
}

static inline
void ascii_encode_o(struct ss_s *ss, char cc, char next) {
	static const unsigned char three = 0x7U;
	const bool nextdigit = !!isdigit((unsigned char)(next));
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		ss_out(ss, '\\');
		for (unsigned ii = 3U * 2U; ii > 0U; ii -= 3U) {
			if (nextdigit || bbv > (char)(1U << ii)) {
				ss_out(ss, xdigits[(bbv >> ii) & three]);
			}
		}
		ss_out(ss, xdigits[bbv & three]);
	}
}

static inline
void ascii_encode_u(struct ss_s *ss, char cc, char next) {
	const bool nextdigit = !!isxdigit((unsigned char)(next));
	ss_out(ss, '\\');
	ss_out(ss, 'u');
	for (unsigned ii = 4U * 3U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (char)(1U << ii)) {
			ss_out(ss, xdigits[(cc >> ii) & four]);
		}
	}
	ss_out(ss, xdigits[cc & four]);
}

static inline
void ascii_encode_U(struct ss_s *ss, char cc, char next) {
	const bool nextdigit = !!isxdigit((unsigned char)(next));
	ss_out(ss, '\\');
	ss_out(ss, 'U');
	for (unsigned ii = 4U * 7U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (char)(1U << ii)) {
			ss_out(ss, xdigits[(cc >> ii) & four]);
		}
	}
	ss_out(ss, xdigits[cc & four]);
}

static inline
char ascii_encode_get_esc(char prev, char cc) {
	switch (cc) {
	case '\'': return '\'';
	case '\"': return '\"';
	case '\a': return 't';
	case '\b': return 'b';
	case '\f': return 'f';
	case '\n': return 'n';
	case '\r': return 'r';
	case '\t': return 't';
	case '\v': return 'v';
	case '\?':
		// If the previous character was ?, then watch out for trigraphs.
		if (prev == '?') {
			return '?';
		}
		break;
	}
	return '\0';
}

#define CSTRLEN(x)  (sizeof(x) - 1)

#define ASCII_ENCODE  ascii_encode_o  // \1\123\377...

static inline
struct ss_s ascii_encode_do(const char str[restrict], size_t str_len, char *restrict newstr, size_t newstr_len) {
	struct ss_s ss_mem = ss_init(newstr);
	struct ss_s *ss = &ss_mem;
	for (char prev = '\0', next = *str, cc = next;
			cc != '\0';
			prev = cc, cc = next) {
		next = *(++str);
		//
		const char esc = ascii_encode_get_esc(prev, cc);
		if (esc != '\0') {
			//dbgln("%c  %#x adding\\%c", cc, cc, esc);
			ss_out(ss, '\\');
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
		assert(ss->newstr == newstr + newstr_len);
	}
	return ss_mem;
}

static inline
size_t ascii_encode_get_length(const char str[restrict], size_t str_len) {
	return ascii_encode_do(str, str_len, NULL, 0).cnt;
}

static inline
int YYIO_printformat_conversion(yio_printctx_t *restrict t,
		const char *restrict *restrict pstr, size_t *pstr_len) {
	//dbgln("%c", t->pf.c_onversion);
	if (t->pf.c_onversion != 'a') return 0;
	const char *restrict str = *pstr;
	const size_t str_len = *pstr_len;
	const size_t newstr_len = ascii_encode_get_length(str, str_len);
	if (newstr_len == str_len) return 0;
	assert(newstr_len > str_len);
	char *const newstr = malloc(newstr_len * sizeof(*newstr));
	if (newstr == NULL) return YIO_ERROR_ENOMEM;
	(void)ascii_encode_do(str, str_len, newstr, newstr_len);
	*pstr_len = newstr_len;
	*pstr = newstr;
	return 1;
}

/* ------------------------------------------------------------------------- */

int YYIO_printformat_generic(yio_printctx_t *restrict t,
		const char str[restrict], size_t str_len, bool is_number, bool is_positive) {
	int err = 0;
	// Detect inf/nan
	const bool is_infnan = is_number && str_len >= 3 && (
			(str[0] == 'i' || str[0] == 'I') ||
			(str[0] == 'n' || str[0] == 'N')
	);
	if (is_infnan) {
		t->pf.grouping = '\0';
		if (t->pf.fill == '0' && t->pf.align == '=') {
			t->pf.fill = ' ';
			t->pf.align = '>';
		}
	}
	const int converted = YYIO_printformat_conversion(t, &str, &str_len);
	if (converted < 0) return converted;
	//
	YYIO_printformat_assert_valid(&t->pf);
	YYIO_printformat_t pf;
	YYIO_printformat_init(&pf, t, str, str_len, is_number, is_positive);
	err = YYIO_printformat_prefix(&pf);
	if (err) goto EXIT;
	err = YYIO_printformat_print(&pf, str, str_len);
	if (err) goto EXIT;
	err = YYIO_printformat_suffix(&pf);
	if (err) goto EXIT;
	//
EXIT:
	if (converted) {
		free((void *)str);
	}
	return err;
}
