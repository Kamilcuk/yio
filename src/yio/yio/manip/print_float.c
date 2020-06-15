/**
 * @file print_float.c
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright 
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
m4_config();
#include "private.h"
#include <ctype.h>
#include <yio/bits/yio_float_strfrom_stupid.h>
#include <yio/bits/yio_float_strfrom_printf.h>
#include <yio/bits/yio_float_strfrom_strfrom.h>

m4_applyforeachdefine(`(
		(f, f   ),
		(f, fpnt),
		( ,     ),
		(l, l   )
)~,`m4_dnl;

int _yIO_print_float$2(yio_printctx_t *t) {
	const char type = t->pf.type ? t->pf.type : 'g';
	const char precision =  t->pf.precision;

	_yIO_FLOAT$1 var = yio_printctx_va_arg_num(t, _yIO_FLOAT$1);
	char *buf = NULL;

	int err = 0;
#ifdef _yIO_HAS_strfrom$1
	err = _yIO_float_astrfrom_strfrom$1(&buf, precision, type, var);
#else // _yIO_HAS_strfrom$1
#ifdef YIO_FLOATS_PREFER_CUSTOM
	err = _yIO_float_astrfrom_stupid$1(&buf, precision, type, var);
#else // YIO_FLOATS_PREFER_CUSTOM
	err = _yIO_float_astrfrom_printf$1(&buf, precision, type, var);
#endif // YIO_FLOATS_PREFER_CUSTOM
#endif // _yIO_HAS_strfrom$1

	if (err) {
		return err;
	}

	const bool is_negative = buf[0] == '-';
	err = _yIO_printformat_number(t, buf + is_negative, !is_negative);
	free(buf);
	return err;
}

~)m4_dnl;
