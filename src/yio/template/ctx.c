/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
#define _XOPEN_SOURCE  1 // wcswidth
#include "private.h"
#if _yIO_HAS_UNISTRING
#include <uniwidth.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

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
	assert(t != NULL);
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = t->out(t->outarg, ptr, size);
	if (ret) return ret;
	t->writtencnt += size;
	return 0;
}

int yπio_printctx_next(yπio_printctx_t *t) {
	assert(t != NULL);
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	assert(*t->ifunc != NULL);
	return (*t->ifunc)(t);
}

struct yπio_printfmt_s *yπio_printctx_get_fmt(yπio_printctx_t *t) {
	return &t->pf;
}

const TCHAR *yπio_printctx_get_fmtstrpnt(yπio_printctx_t *t) {
	return t->fmt;
}

va_list *_yΩIO_printctx_get_va(yπio_printctx_t *t) {
	return t->va;
}

int _yΩIO_printctx_print(yπio_printctx_t *t, yπio_printdata_t *data, const TCHAR *fmt, ...) {
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
#if {{(MODE == 1)|int}} && _yIO_HAS_UNISTRING
	return u8_width((const uint8_t*)str, str_len, locale_charset());
#elif {{(MODE == 2)|int}} && _yIO_HAS_wcswidth
	const int width = wcswidth(str, str_len);
	return width < 0 ? str_len : (size_t)width;
#elif {{(MODE == 3)|int}}
	return u16_width(str, str_len, locale_charset());
#elif {{(MODE == 4)|int}}
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
const TCHAR *_yΩIO_str_comma_or_end(const TCHAR str[], size_t len) {
	for (; len-- && str[0] != TC('.'); ++str) {
		continue;
	}
	return str;
}

static inline
int _yΩIO_print_format_generic_number_grouping(yπio_printctx_t *t, TCHAR grouping,
		const TCHAR str[], size_t str_len) {
	int err = 0;
	// Find end of string (integer) or comma (floating point)
	const TCHAR * const str0 = str;
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
	// if there are more characters to write
	if (str != comma_or_end) {
		while (1) {
			// we get here either by writing first block and printing grouping
			// or by printing nothing by far
			// or by the loop
			err = yπio_printctx_raw_write(t, str, blocksize);
			str += blocksize;
			if (err) return err;
			// we check if the end is between grouping and end
			assert(str <= comma_or_end);
			if (str == comma_or_end) {
				break;
			}
			// print the grounping between
			err = yπio_printctx_raw_write(t, &grouping, 1);
			if (err) return err;
		}
	}

	assert(str >= str0);
	const size_t written_so_far = str - str0;
	assert(str_len >= written_so_far);
	const size_t left_to_write = str_len - written_so_far;
	if (left_to_write) {
		err = yπio_printctx_raw_write(t, str, left_to_write);
		if (err) return err;
	}

	return err;
}

static inline
int _yΩIO_printformat_print(_yΩIO_printformat_t *pf, const TCHAR str[], size_t str_len) {
	yπio_printctx_t * const t = pf->t;
	struct yπio_printfmt_s * const f = &pf->t->pf;
	const bool is_number = pf->is_number;
	const TCHAR grouping = f->grouping;

	if (is_number == true && grouping != YΩIO_GROUPING_NONE) {
		const int err = _yΩIO_print_format_generic_number_grouping(t, grouping, str, str_len);
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
	assert(is_one_of_or_nul(pf->grouping, TC("_,")));
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

{% if MODEX != 1 %}
int _yΩIO_printformat_generic_char(yπio_printctx_t *t,
		const char str[], size_t str_len, bool is_number, bool is_positive) {
	const TCHAR *dest;
	size_t dest_len;
	int ret = _yIO_strconv_str_to_πstr(str, str_len, &dest, &dest_len);
	if (ret) return ret;
	ret = _yΩIO_printformat_generic(t, dest, dest_len, is_number, is_positive);
	_yIO_strconv_free_str_to_πstr(str, dest);
	return ret;
}
{% endif %}
