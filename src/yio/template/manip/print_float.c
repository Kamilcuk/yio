/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_float.h"
#include <ctype.h>

#if __CDT_PARSER__
#define _yIO_float_astrfrom_$2$1  _yIO_float_astrfrom_customd
#define _yIO_FLOAT$3  double
#endif

// m4_generate_print_floats(type, suffix, promoted_type<for float it's double> )
m4_define_function(«m4_generate_print_floats», m4_syncline(1)«m4_dnl);

static inline
int _yΩIO_print_float_$2$3_in(yπio_printctx_t *t, _yIO_FLOAT$3 var);

static inline
int _yΩIO_print_float_$2$1_in(yπio_printctx_t *t, _yIO_FLOAT$1 var) {
	int err = yπio_printctx_init(t);
	if (err) return err;
	const char type = t->pf.type ? t->pf.type : 'g';
	const int precision =  t->pf.precision;
	_yIO_CACHE_AUTO_DECL(_result, result, length);
	err = _yIO_float_astrfrom_$2$1(&result, &length, precision, type, var);
	if (err) return err;
	if (isdigit((unsigned char)result[0])) { // TODO: There's something wrong here.
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
	return _yΩIO_print_float_$2$3_in(t, var);
}


#if 0
m4_ifelse(«$1», «f», m4_syncline(1)«m4_dnl;
int _yΩIO_print_float_$2$1pnt(yπio_printctx_t *t) {
	const _yIO_FLOAT$1 var = * yπio_printctx_va_arg(t, _yIO_FLOAT$1 *);
	return _yΩIO_print_float_$2$1_in(t, var);
}
») m4_dnl;
#endif

») m4_dnl;

m4_ifdef(«m4_floatlist», «»,
«m4_define(«m4_floatlist», «
		(f),(d),(l)
»)
»)

m4_applysync((m4_floatlist),
«

m4_define(«m4_arg2», «m4_ifelse(«$1», «f», «d», «$1»)»)

#ifndef _yIO_HAS_FLOAT$1
#error  _yIO_HAS_FLOAT$1
#endif
#if _yIO_HAS_FLOAT$1

m4_generate_print_floats($1, strfrom, m4_arg2)

#if _yIO_has_float_custom$1
m4_generate_print_floats($1, custom, m4_arg2)
#endif

#if _yIO_has_float_printf$1
m4_generate_print_floats($1, printf, m4_arg2)
#endif

#endif

»)

