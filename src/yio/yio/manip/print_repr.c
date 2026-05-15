/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "../../private/yio_buf.h"
#include "print_repr.h"
#include "../ctx.h"
#include "private.h"
#include <assert.h>
#include <stdlib.h>

static inline
bool is_print_ascii(char tcc) {
	const unsigned char ascii_min_printable = (unsigned char)' ';
	const unsigned char ascii_max_printable = 126U;
	const unsigned char cc = tcc;
	return ascii_min_printable <= cc && cc <= ascii_max_printable;
}

struct ss_s {
	YIO_buf *str;
};

static inline
int ss_out(struct ss_s *t, char cc) {
	return YIO_buf_putc(t->str, cc);
}

#define xdigits YIO_digit_to_hex
static const unsigned char four = 0xfU;

static inline
int ascii_encode_hex(struct ss_s *ss, char cc, char next) {
	static_assert(CHAR_BIT == 8, "Really? TODO");
	const bool nextdigit = YIO_isxdigit(next);
	int err;
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		err = ss_out(ss, '\\');
		if (err) return err;
		err = ss_out(ss, 'x');
		if (err) return err;
		if (nextdigit || bbv > (char)16U) {
			err = ss_out(ss, xdigits[(bbv >> 4U) & four]);
			if (err) return err;
		}
		err = ss_out(ss, xdigits[bbv & four]);
		if (err) return err;
	}
	return 0;
}

static inline
int ascii_encode_oct(struct ss_s *ss, char cc, char next) {
	static const unsigned char three = 0x7U;
	const bool nextdigit = YIO_isdigit(next);
	int err;
	for (const unsigned char *bb = (const unsigned char *)&cc, *bbend = bb + sizeof(cc);
			bb != bbend; ++bb) {
		const unsigned char bbv = *bb;
		//dbgln("?  %#x ? %#x adding \\xbyte", cc, bbv);
		err = ss_out(ss, '\\');
		if (err) return err;
		for (unsigned ii = 3U * 2U; ii > 0U; ii -= 3U) {
			if (nextdigit || bbv > (char)(1U << ii)) {
				err = ss_out(ss, xdigits[(bbv >> ii) & three]);
				if (err) return err;
			}
		}
		err = ss_out(ss, xdigits[bbv & three]);
		if (err) return err;
	}
	return 0;
}

static inline
int ascii_encode_caret(struct ss_s *ss, char cc, char next) {
	(void)next;
	int err;
  const unsigned char uc = (unsigned char)cc;
  if (uc < 32) {
    // Control characters 0-31 map to ^@ through ^_
    err = ss_out(ss, '^');
    if (err) return err;
    err = ss_out(ss, (char)(uc + 64));
    if (err) return err;
  } else if (uc == 127) {
    // DEL (127) maps to ^?
    err = ss_out(ss, '^');
    if (err) return err;
    err = ss_out(ss, '?');
    if (err) return err;
  } else {
    // Printable ASCII or High-bit characters (128+)
    // Caret notation is usually only defined for 7-bit ASCII.
    // We pass printable chars through as-is.
    err = ss_out(ss, (char)uc);
    if (err) return err;
  }
  return 0;
}

static inline
int ascii_encode_u(struct ss_s *ss, char cc, char next) {
	int err;
	const bool nextdigit = YIO_isxdigit(next);
	err = ss_out(ss, '\\');
	if (err) return err;
	err = ss_out(ss, 'u');
	if (err) return err;
	for (unsigned ii = 4U * 3U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (char)(1U << ii)) {
			err = ss_out(ss, xdigits[(cc >> ii) & four]);
			if (err) return err;
		}
	}
	err = ss_out(ss, xdigits[cc & four]);
	if (err) return err;
	return 0;
}

static inline
int ascii_encode_U(struct ss_s *ss, char cc, char next) {
	int err;
	const bool nextdigit = YIO_isxdigit(next);
	err = ss_out(ss, '\\');
	if (err) return err;
	err = ss_out(ss, 'U');
	if (err) return err;
	for (unsigned ii = 4U * 7U; ii > 0U; ii -= 4U) {
		if (nextdigit || cc > (char)(1U << ii)) {
			err = ss_out(ss, xdigits[(cc >> ii) & four]);
			if (err) return err;
		}
	}
	err = ss_out(ss, xdigits[cc & four]);
	if (err) return err;
	return 0;
}

static inline
char ascii_encode_get_esc(char prev, char cc) {
	switch (cc) {
	case '\'': return '\'';
	case '\"': return '\"';
	case '\a': return 'a';
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

static inline
int ascii_encode_do(YIO_buf *out, int (*encoder)(struct ss_s *ss, char cc, char next) YIO_REENTRANT,
										const char *str, size_t str_len, bool use_esc) {
	int err;
	struct ss_s ss_mem = {out}, *ss = &ss_mem;
	const char *end = str + str_len;
	for (char prev = '\0', next = *str, cc = next; str != end; prev = cc, cc = next) {
		next = *(++str);
		//
		const char esc = use_esc ? ascii_encode_get_esc(prev, cc) : '\0';
		if (esc != '\0') {
			//dbgln("%c  %#x adding\\%c", cc, cc, esc);
			err = ss_out(ss, '\\');
			if (err) return err;
			err = ss_out(ss, esc);
			if (err) return err;
		} else if (is_print_ascii(cc)) {
			//dbgln("%c  %#x is_ascii", cc, cc);
			err = ss_out(ss, cc);
			if (err) return err;
		} else {
			err = encoder(ss, cc, next);
			if (err) return err;
		}
	}
	return 0;
}

{% set modes = ["hex", "oct", "caret", "u", "U"] %}
{% call(V) j_FOREACHAPPLY(modes) %}#line
static int print_repr_$1_in(void *arg, const char *ptr, size_t count) {
	const bool use_esc = {{ 0 if V.1 == "caret" else 1 }};
	return ascii_encode_do(arg, ascii_encode_$1, ptr, count, use_esc);
}
int YIO_print_repr_$1(yio_printctx_t *t) {
	int val = yio_printctx_va_arg(t, int);
	(void)val;
	int err = yio_printctx_init(t);
	if (err) return yio_printctx_err_skip(t, err, 1);
	YIO_buf str;
	YIO_buf_init(&str);
	const yio_printdata_t data[] = {*t->ifunc++, 0};
	err = yio_vbprintf(print_repr_$1_in, &str, data, 0, t->va);
	if (err < 0) goto EXIT;
	err = yio_printctx_put(t, YIO_buf_data(&str), YIO_buf_len(&str));
EXIT:
	YIO_buf_fini(&str);
	return err;
}
{% endcall %}
