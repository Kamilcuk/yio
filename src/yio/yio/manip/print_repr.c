/**
 * @file
 * @date 2020-06-14
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
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
	char *newstr;
	size_t cnt;
};

static inline void ss_init(struct ss_s *ss, char *newstr) {
	ss->newstr = newstr;
	ss->cnt = 0;
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
	const bool nextdigit = YYIO_isxdigit(next);
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
	const bool nextdigit = YYIO_isdigit(next);
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
	const bool nextdigit = YYIO_isxdigit(next);
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
	const bool nextdigit = YYIO_isxdigit(next);
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
size_t ascii_encode_do(const char *str, size_t str_len, char *newstr, size_t newstr_len) {
	struct ss_s ss_mem;
	struct ss_s *ss = &ss_mem;
	ss_init(ss, newstr);
	const char *end = str + str_len;
	for (char prev = '\0', next = *str, cc = next; str != end; prev = cc, cc = next) {
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
			ascii_encode_x(ss, cc, next);
		}
	}
	if (newstr != NULL) {
		(void)newstr_len;
		assert(ss->newstr == newstr + newstr_len);
	}
	return ss_mem.cnt;
}

static inline
int YYIO_printformat_conversion(yio_printctx_t *t,
		const char **pstr, size_t *pstr_len) {
	//dbgln("%c", t->pf.c_onversion);
	if (t->pf.c_onversion != 'a') return 0;
	const char *str = *pstr;
	const size_t str_len = *pstr_len;
	const size_t newstr_len = ascii_encode_do(str, str_len, NULL, 0);
	if (newstr_len == str_len) return 0;
	assert(newstr_len > str_len);
	char *const newstr = malloc(newstr_len * sizeof(*newstr));
	if (newstr == NULL) return YIO_ERROR_ENOMEM;
	(void)ascii_encode_do(str, str_len, newstr, newstr_len);
	*pstr_len = newstr_len;
	*pstr = newstr;
	return 1;
}

{% set modes = ["hex", "oct", "caret", "u", "U"] %}
{% call() j_FOREACHAPPLY(modes) %}#line
int YYIO_print_repr_$1(yio_printctx_t *t) {
	int val = yio_printctx_va_arg(t, int);
	int err = yio_printctx_init(t);
	if (err) return err;
	(void)val;
	return YIO_ERROR_ENOSYS;
}
{% endcall %}
