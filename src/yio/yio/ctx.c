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
#include "yio/yio_error.h"
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
#include <limits.h>
#include <stdlib.h>
#ifndef YIO_ENABLE_LOCALE
#error YIO_ENABLE_LOCALE
#endif
#if YIO_ENABLE_LOCALE
#include <langinfo.h>
#endif
#ifndef YIO_HAS_WCHAR_H
#error
#endif
#if YIO_HAS_WCHAR_H
#include <wchar.h>
#endif

/* ------------------------------------------------------------------------- */

static const char YYIO_ALIGN_LEFT = '<';
static const char YYIO_ALIGN_RIGHT = '>';
static const char YYIO_ALIGN_PADSIGN = '=';
static const char YYIO_ALIGN_CENTER = '^';

static const char YYIO_SIGN_ALWAYS = '+';
//static const char YYIO_SIGN_NEGATIVE = '-';
static const char YYIO_SIGN_ALWAYSSPACE = ' ';

static const uint8_t YYIO_LIMIT_MAX = 254;

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
	assert(YYIO_isdigit(d));
	return d - '0';
}

int YYIO_printctx_strtoi_noerr(const char **fmtpnt) {
	const char *fmt = *fmtpnt;
	assert(YYIO_isdigit(fmt[0]));
	int num = 0;
	do {
		assert(num < INT_MAX / 10);
		num *= 10;
		const int c = YYIO_digit_to_number(fmt[0]);
		assert(num < INT_MAX - c);
		num += c;
		++fmt;
	} while (YYIO_isdigit(fmt[0]));
	*fmtpnt = fmt;
	return num;
}

#if YIO_ENABLE_DYNAMIC_PFMT
static inline
int YYIO_printctx_take_positional_param(yio_printctx_t *t, const char *fmt, const char **endptr, uint8_t *res) {
	assert(fmt[0] == '{');
	fmt++;
	if (YYIO_isdigit(fmt[0])) {
		YYIO_skip_arm(t, (unsigned int)YYIO_printctx_strtoi_noerr(&fmt));
		const int skiperr = YYIO_skip_do(t);
		if (skiperr) return skiperr;
	}
	if (t->ifunc == NULL) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NO_ARGS, "no arguments for positional width or precision");
	}
	int num;
	const yio_printdata_t ifunc = *t->ifunc++;
	// TODO: conversions
	if (ifunc == &YYIO_print_bool)        num = yio_printctx_va_arg_promote(t, bool);
	else if (ifunc == &YYIO_print_char)   num = yio_printctx_va_arg_promote(t, char);
	else if (ifunc == &YYIO_print_schar)  num = yio_printctx_va_arg_promote(t, signed char);
	else if (ifunc == &YYIO_print_uchar) num = yio_printctx_va_arg_promote(t, unsigned char);
	else if (ifunc == &YYIO_print_short)  num = yio_printctx_va_arg_promote(t, short);
	else if (ifunc == &YYIO_print_ushort) num = yio_printctx_va_arg_promote(t, unsigned short);
	else if (ifunc == &YYIO_print_int)    num = yio_printctx_va_arg(t, int);
	else if (ifunc == &YYIO_print_uint)   num = yio_printctx_va_arg(t, unsigned int); // NOLINT
	else if (ifunc == &YYIO_print_long)   num = yio_printctx_va_arg(t, long);
	else if (ifunc == &YYIO_print_ulong)  num = yio_printctx_va_arg(t, unsigned long);
#if YYIO_HAS_LLONG
	else if (ifunc == &YYIO_print_llong)  num = yio_printctx_va_arg(t, long long);
	else if (ifunc == &YYIO_print_ullong) num = yio_printctx_va_arg(t, unsigned long long);
#endif
#if YYIO_HAS_INT128
	else if (ifunc == &YYIO_print___int128)  num = yio_printctx_va_arg(t, __int128);
	else if (ifunc == &YYIO_print_u__int128) num = yio_printctx_va_arg(t, unsigned __int128);
#endif
	else return YYIO_ERROR(YIO_ERROR_POSITIONAL_NOT_NUMBER, "positional width or precision specifier is not a number");
	if (fmt++[0] != '}') {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_MISSING_RIGHT_BRACE, "missing '}' when parsing positional width or precision specifier");
	}
	if (num < 0) {
		return YYIO_ERROR(YIO_ERROR_POSITIONAL_NEGATIVE, "positional width or precision specifier cannot be negative");
	}
	*endptr = fmt;
	*res = (num > YYIO_LIMIT_MAX ? YYIO_LIMIT_MAX : num) + 1;
	return 0;
}
#endif // YIO_ENABLE_DYNAMIC_PFMT

int YYIO_printctx_stdintparam(yio_printctx_t *t, const char *fmt, const char **endptr, uint8_t *res) {
	(void)t;
#if YIO_ENABLE_DYNAMIC_PFMT
	if (fmt[0] == '{') {
		return YYIO_printctx_take_positional_param(t, fmt, endptr, res);
	}
#endif // YIO_ENABLE_DYNAMIC_PFMT
	if (YYIO_isdigit(fmt[0])) {
		const int num = YYIO_printctx_strtoi_noerr(&fmt);
		*res = (num > YYIO_LIMIT_MAX ? YYIO_LIMIT_MAX : num) + 1;
		*endptr = fmt;
	} else {
		// do nothing
		*endptr = fmt;
	}
	return 0;
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
	if (fmt[0] != '\0' && YYIO_ANYEQ(fmt[1], '<', '>', '=', '^')) {
		pf->fill = fmt++[0];
		pf->align = fmt++[0];
	} else if (YYIO_ANYEQ(fmt[0], '<', '>', '=', '^')) {
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
		#if YIO_ENABLE_DYNAMIC_PFMT
		case '{':
			--fmt;
			ret = YYIO_printctx_stdintparam(t, fmt, &fmt, &pf->width);
			if (ret) goto EXIT;
			break;
		#endif
		case '_':
		case ',':
		case 'L':
			#if YIO_ENABLE_DIGIT_GROUPING
			pf->grouping = ch;
			#endif
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
		case 'u':
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

int yio_printctx_raw_write(yio_printctx_t *t, const char *ptr, size_t size) {
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

int YYIO_printctx_print_in(yio_printctx_t *t, const yio_printdata_t *data, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	const int ret = yio_vbprintf(t->out, t->outarg, data, fmt, &va);
	va_end(va);
	if (ret < 0) {
		return ret;
	}
	t->writtencnt += ret;
	return 0;
}

/* printformat --------------------------------------------------- */

#ifndef YYIO_HAS_wcwidth
#error
#endif
#if YIO_HAS_WCHAR_H
static size_t YYIO_mbwidth(const char *str, size_t str_len) {
	mbstate_t st;
  memset(&st, 0, sizeof(st));
  const char *p = str;
  size_t rem = str_len;
  size_t total = 0;
  while (rem > 0) {
    wchar_t wc;
    size_t r = mbrtowc(&wc, p, rem, &st);
    if (r == (size_t)-1) {
      // invalid byte
      ++p; --rem;
      ++total;
      memset(&st, 0, sizeof(st));
    } else if (r == (size_t)-2) {
      // incomplete sequence
      break;
    } else if (r == 0) {
      // null
      ++p; --rem;
    } else {
#if YYIO_HAS_wcwidth || defined(wcwidth)
      int w = wcwidth(wc);
      total += (w > 0) ? w : (w < 0 ? 1 : 0);
#else
      total += 1;
#endif
      p += r;
      rem -= r;
    }
  }
  return total;
}
#endif

#ifndef YYIO_HAS_UNISTRING
#error
#endif
static inline
size_t YYIO_width(const char *str, size_t str_len) {
#if YYIO_HAS_UNISTRING
	return u8_width((const uint8_t*)str, str_len, locale_charset());
#elif YIO_HAS_WCHAR_H
	return YYIO_mbwidth(str, str_len);
#else
	(void)str;
	return str_len;
#endif
}

static inline
int YYIO_printctx_pad_write(yio_printctx_t *t, char fill, size_t count) {
	if (count == 0) return 0;
	char buf[YYIO_INIT_CAPACITY];
	memset(buf, fill, count < sizeof(buf) ? count : sizeof(buf));
	while (count > 0) {
		const size_t chunk = count < sizeof(buf) ? count : sizeof(buf);
		const int err = yio_printctx_raw_write(t, buf, chunk);
		if (err) return err;
		count -= chunk;
	}
	return 0;
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
	pf->t = t;
	pf->str_len = (is_number ? str_len : YYIO_width(str, str_len));
	pf->is_number = is_number;
	pf->is_positive = is_positive;
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

	const bool has_hash = is_number && f->hash &&
					YYIO_ANYEQ(f->type, 'x', 'X', 'o', 'O', 'b', 'B');
	const bool has_sign = is_number && (f->sign == YYIO_SIGN_ALWAYS ||
					f->sign == YYIO_SIGN_ALWAYSSPACE || is_positive == false);
	const size_t alllen = len + (size_t)( 2U * has_hash + has_sign );
	*alllen0 = alllen;
	const size_t width = yio_width_get_default(f->width, 0);

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
		const size_t diff = f->align == YYIO_ALIGN_CENTER ? tmp / 2 : tmp;
		const int err = YYIO_printctx_pad_write(t, f->fill ? f->fill : ' ', diff);
		if (err) return err;
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
	const size_t width = yio_width_get_default(f->width, 0);
	if ((f->align == YYIO_ALIGN_LEFT || f->align == YYIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		const size_t diff = f->align == YYIO_ALIGN_CENTER ? tmp / 2 + (tmp % 2) : tmp;
		return YYIO_printctx_pad_write(t, f->fill ? f->fill : ' ', diff);
	}
	return 0;
}

#if YIO_ENABLE_DIGIT_GROUPING
static inline
const char *str_dot_or_end(const char str[], size_t len) {
	for (; len != 0 && str[0] != '.' && str[0] != ','; ++str, --len) {
	}
	return str;
}

#if YIO_ENABLE_LOCALE
static const char NOGROUP[1] = { CHAR_MAX };
#endif
static const char GROUP3[2] = "\x03";
static const char GROUP4[2] = "\x04";

static inline
const char *get_group(yio_printctx_t *t) {
#if YIO_ENABLE_LOCALE && defined(GROUPING)
	if (t->pf.grouping == 'L') {
		const char *r = nl_langinfo(GROUPING);
		return r != NULL && *r != '\0' ? r : NOGROUP;
	}
#endif
	//dbgln("HERE %c", (int)t->pf.type);
	return YYIO_ANYEQ(t->pf.type, 'b', 'B', 'x', 'X') ? GROUP4 : GROUP3;
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
#if YIO_ENABLE_LOCALE
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
#if YIO_ENABLE_LOCALE
	if (t->pf.grouping == 'L') {
		const char *dot = nl_langinfo(RADIXCHAR); // DECIMAL_POINT
		if (dot == NULL) return 0;
		return yio_printctx_raw_write(t, dot, strlen(dot));
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
#endif

static inline
int YYIO_printformat_print(YYIO_printformat_t *pf, const char str[], size_t str_len) {
	yio_printctx_t * const t = pf->t;
#if YIO_ENABLE_DIGIT_GROUPING
	struct yio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	if (is_number == true && f->grouping != '\0') {
		return YYIO_print_format_generic_number_grouping(t, str, str_len);
	}
#endif
	return yio_printctx_raw_write(t, str, str_len);
}

static inline
void YYIO_printformat_assert_valid(const struct yio_printfmt_s *pf) {
	(void)pf;
	assert(YYIO_ANYEQ(pf->align, 0, '<', '>', '=', '^'));
	assert(YYIO_ANYEQ(pf->sign, 0, '+', '-', ' '));
	assert(!YYIO_ANYEQ(pf->fill, '{', '}'));
	assert(YYIO_ANYEQ(pf->grouping, 0, '_', ',', 'L'));
}

/* ------------------------------------------------------------------------- */

int YYIO_printformat_generic(yio_printctx_t *t,
		const char *str, size_t str_len, bool is_number, bool is_positive) {
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
	//
	YYIO_printformat_assert_valid(&t->pf);
	YYIO_printformat_t pf;
	YYIO_printformat_init(&pf, t, str, str_len, is_number, is_positive);
	int err = YYIO_printformat_prefix(&pf);
	if (err) return err;
	err = YYIO_printformat_print(&pf, str, str_len);
	if (err) return err;
	return YYIO_printformat_suffix(&pf);
}

