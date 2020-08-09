/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include <ctype.h>

// m4_generate_print_floats(type, suffix)
m4_define(`m4_generate_print_floats~, `m4_dnl;

static inline
int _yΩIO_print_float_$2_in(yπio_printctx_t *t, _yIO_FLOAT$1 var) {
	const char type = t->pf.type ? t->pf.type : 'g';
	const int precision =  t->pf.precision;
	char *buf = NULL;
	const int len = _yIO_float_astrfrom_$2(&buf, precision, type, var);
	if (len < 0) return len;
	const bool is_negative = buf[0] == '-';
	int err = yπio_printctx_put_number(t, buf + is_negative, len - is_negative, !is_negative);
	free(buf);
	return err;
}

int _yΩIO_print_float_$2(yπio_printctx_t *t) {
	const _yIO_FLOAT$1 var = yπio_printctx_va_arg_num(t, _yIO_FLOAT$1);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return _yΩIO_print_float_$2_in(t, var);
}

m4_ifelse(`$1~, `f~, `m4_dnl;

int _yΩIO_print_float_$2pnt(yπio_printctx_t *t) {
	const _yIO_FLOAT$1 var = * yπio_printctx_va_arg(t, _yIO_FLOAT$1 *);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return _yΩIO_print_float_$2_in(t, var);
}

~) m4_dnl;

~) m4_dnl;

m4_applyforeachdefine((
		(f, strfromf),
		(, strfrom),
		(l, strfroml)
), `m4_dnl;
#ifdef _yIO_HAS_$2
m4_generate_print_floats($1, $2)
#endif
~) m4_dnl;

m4_applyforeachdefine((
		m4_applyforeachdefine((
				(f),
				(),
				(l)
			),
			`($1, stupid$1),($1, printf$1)~,
			`,~)), `m4_dnl;
m4_generate_print_floats($1, $2)
~) m4_dnl;
