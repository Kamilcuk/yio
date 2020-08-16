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
m4_define(`m4_generate_print_floats~, m4_syncline(1)`m4_dnl;

static inline
int _yΩIO_print_float_$2$3_in(yπio_printctx_t *t, _yIO_FLOAT$3 var);

static inline
int _yΩIO_print_float_$2$1_in(yπio_printctx_t *t, _yIO_FLOAT$1 var) {
	const char type = t->pf.type ? t->pf.type : 'g';
	const int precision =  t->pf.precision;
	_yIO_CACHE_AUTO_DECL(_result, result, length);
	int err = 0;
	err = _yIO_float_astrfrom_$2$1(&result, &length, precision, type, var);
	if (err) return err;
	if (isdigit((unsigned char)result[0])) {
		const bool is_negative = result[0] == '-';
		err = yπio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
	} else {
		err = yπio_printctx_put(t, result, length);
	}
	_yIO_CACHE_AUTO_FREE(_result, result, length);
	return err;
}

int _yΩIO_print_float_$2$1(yπio_printctx_t *t) {
	const _yIO_FLOAT$3 var = yπio_printctx_va_arg2(t, _yIO_FLOAT$1, _yIO_FLOAT$3);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return _yΩIO_print_float_$2$3_in(t, var);
}

m4_ifelse(`$1~, `f~, m4_syncline(1)`m4_dnl;

int _yΩIO_print_float_$2$1pnt(yπio_printctx_t *t) {
	const _yIO_FLOAT$1 var = * yπio_printctx_va_arg(t, _yIO_FLOAT$1 *);
	const int err = yπio_printctx_init(t);
	if (err) return err;
	return _yΩIO_print_float_$2$1_in(t, var);
}

~) m4_dnl;

~) m4_dnl;

m4_applyforeachdefine(((f, d),(d, d),(l, l)), m4_syncline(1)`m4_dnl;
#ifndef _yIO_HAS_strfrom$1
#error
#endif
#if _yIO_HAS_strfrom$1
m4_generate_print_floats($1, strfrom, $2)
#endif
~) m4_dnl;

m4_applyforeachdefine((
		m4_applyforeachdefine(((f, d),(d, d),(l, l)),
			`($1, stupid, $2),($1, printf, $2)~,
			`,~)), `m4_dnl;
m4_generate_print_floats($1, $2, $3)
~) m4_dnl;

