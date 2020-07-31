/**
 * @file
 * @date 5 kwi 2020
 * @author Kamil Cukrowski
 * @copyright GPL-3.0-only
 * SPDX-License-Identifier: GPL-3.0-only
 */
m4_config_yio_template(`m4_dnl);
#include "private.h"
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

/* printctx ---------------------------------------------------- */

int yπio_printctx_init(yπio_printctx_t *t) {
	if (t->fmt) {
		const int err = _yIO_pfmt_parse(t, &t->pf, t->fmt, &t->fmt);
		if (err) return err;
	}
	return 0;
}

int yπio_printctx_raw_write(yπio_printctx_t *t, const Ychar *ptr, size_t size) {
	assert(t != NULL);
	assert(t->out != NULL);
	assert(ptr != NULL);
	const int ret = t->out(t->outarg, ptr, size);
	if (ret) return ret;
	assert(SIZE_MAX - size > t->writtencnt);
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

va_list *_yΩIO_printctx_inc_va_list(yπio_printctx_t *t, size_t sizeof_realtype) {
	return _yΩIO_commonctx_inc_va_list(&t->c, sizeof_realtype);
}

int _yΩIO_printctx_print(yπio_printctx_t *t, yπio_printdata_t *data, ...) {
	va_list va;
	va_start(va, data);
	const int ret = yπvbprintf(t->out, t->outarg, data, &va);
	va_end(va);
	if (ret < 0) {
		return ret;
	}
	assert(t->writtencnt < (size_t)INT_MAX - ret);
	t->writtencnt += ret;
	return 0;
}

/* printformat --------------------------------------------------- */

typedef struct _yIO_printformat_t {
	yπio_printctx_t *t;
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
_yIO_printformat_t _yIO_printformat_init(yπio_printctx_t *t,
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
int _yIO_printformat_prefix__print_sign_hash(yπio_printctx_t *t,
		const struct yio_printfmt_s *f,
		bool has_sign, bool has_hash, bool is_positive) {
	if (has_sign) {
		const char c = is_positive ? f->sign : Yc('-');
		const int err = yio_printctx_raw_write(t, &c, 1);
		if (err) return err;
	}
	if (has_hash) {
		assert(Yisprint(f->type));
		const Ychar buf[2] = { Yc('0'), f->type, };
		const int err = yio_printctx_raw_write(t, buf, sizeof(buf));
		if (err) return err;
	}
	return 0;
}

static inline
int _yIO_printformat_prefix(_yIO_printformat_t *pf) {
	yπio_printctx_t * const t = pf->t;
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
			const int err = yio_printctx_raw_write(t, &f->fill, 1);
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
	yπio_printctx_t * const t = pf->t;
	struct yio_printfmt_s * const f = &pf->t->pf;
	const size_t alllen = pf->alllen;

	const size_t width = f->width > 0 ? f->width : 0;
	if ((f->align == YIO_ALIGN_LEFT || f->align == YIO_ALIGN_CENTER) && width > alllen) {
		const size_t tmp = (width - alllen);
		size_t diff = f->align == YIO_ALIGN_CENTER ? tmp / 2: tmp;
		while (diff--) {
			const int err = yio_printctx_raw_write(t, &f->fill, 1);
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
		err = yio_printctx_raw_write(t, str, firstblock);
		str += firstblock;
		if (err) return err;
		// if there are more numbers to write, write the grouping character
		if (str != comma_or_end) {
			err = yio_printctx_raw_write(t, &grouping, 1);
			if (err) return err;
		}
	}
	// if there are more characters to write
	if (str != comma_or_end) {
		while (1) {
			// we get here either by writing first block and printing grouping
			// or by printing nothing by far
			// or by the loop
			err = yio_printctx_raw_write(t, str, blocksize);
			str += blocksize;
			if (err) return err;
			// we check if the end is between grouping and end
			assert(str <= comma_or_end);
			if (str == comma_or_end) {
				break;
			}
			// print the grounping between
			err = yio_printctx_raw_write(t, &grouping, 1);
			if (err) return err;
		}
	}

	assert(str >= str0);
	const size_t written_so_far = str - str0;
	assert(str_len >= written_so_far);
	const size_t left_to_write = str_len - written_so_far;
	if (left_to_write) {
		err = yio_printctx_raw_write(t, str, left_to_write);
		if (err) return err;
	}

	return err;
}

static inline
int _yIO_printformat_print(_yIO_printformat_t *pf, const Ychar str[]) {
	yπio_printctx_t * const t = pf->t;
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
		const int err = yio_printctx_raw_write(t, str, to_print);
		if (err) return err;
	}

	return 0;
}

int _yIO_printformat_generic(yπio_printctx_t * restrict t,
		size_t str_len, const char str[restrict], bool is_number, bool is_positive) {
	_yIO_printformat_t pf = _yIO_printformat_init(t, str_len, is_number, is_positive);
	int err = _yIO_printformat_prefix(&pf);
	if (err) return err;
	err = _yIO_printformat_print(&pf, str);
	if (err) return err;
	err = _yIO_printformat_suffix(&pf);
	if (err) return err;
	return 0;
}

/* scanctx ---------------------------------------------------- */

int yπio_scanctx_in(yio_scanctx_t *t, int *c) {
	assert(t != NULL);
	if (t->unin) {
		t->unin = false;
		*c = t->ch;
		// _yIO_dbgln("in back '%s'", _yIO_printc(t->ch));
		return 0;
	}
	const int ret = t->in(t->inarg, &t->ch);
	*c = t->ch;
	// _yIO_dbgln("in '%s' %zu ret=%d", _yIO_printc(t->ch), t->scannedcnt, ret);
	if (ret) return ret;
	assert(SIZE_MAX - 1 > t->scannedcnt);
	++t->scannedcnt;
	return 0;
}

void yπio_scanctx_unin(yio_scanctx_t *t) {
	assert(t->unin == false);
	t->unin = true;
	// _yIO_dbgln("unin '%s'", _yIO_printc(t->ch));
}

int yπio_scanctx_next(yio_scanctx_t *t) {
	assert(t != NULL);
	assert(t->ifunc != NULL);
	assert(*t->ifunc != NULL);
	++t->ifunc;
	assert(*t->ifunc != NULL);
	return (*t->ifunc)(t);
}

struct yio_scanfmt_s *yπio_scanctx_get_fmt(yio_scanctx_t *t) {
	return &t->sf;
}

size_t yπio_scanctx_arg_size_next(yio_scanctx_t *t) {
	assert(t != NULL);
	assert(t->argpntsizespnt != NULL);
	assert(*t->argpntsizespnt != 0 ||
			!"you called yio_scanctx_arg_size_next too many times");
	return *t->argpntsizespnt++;
}

va_list *_yΩIO_scanctx_inc_va_list(yio_scanctx_t *t, size_t sizeof_realtype) {
	return _yΩIO_commonctx_inc_va_list(&t->c, sizeof_realtype);
}

int _yΩIO_scanctx_scan(yio_scanctx_t *t, yπio_scandata_t *data, ...) {
	assert(t != NULL);
	va_list va;
	va_start(va, data);
	struct yio_scanret_s ret = yπvbscanf(t->in, t->inarg, data, &va);
	va_end(va);
	assert(t->scannedcnt < (size_t)INT_MAX - ret.count);
	t->scannedcnt += ret.count;
	if (ret.lastchar != EOF) {
		t->unin = true;
		t->ch = ret.lastchar;
	}
	return ret.error;
}

~)m4_dnl;

