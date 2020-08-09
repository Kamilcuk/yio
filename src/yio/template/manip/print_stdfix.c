/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "print_stdfix.h"

#ifdef _yIO_HAS_STDFIX_TYPES

#include "../../private/yio_stdfix.h"
#include "../../private/yio_stdfix_strfrom.h"

m4_divert(-1)
// `m4_stdfix_types~
m4_include(`yio/private/yio_stdfix.h~)
m4_divert(0)m4_dnl;

m4_applyforeachdefine((m4_stdfix_types), `m4_dnl;

int _yΩIO_print_$3(yπio_printctx_t *t) {
	const _yIO_TYPE_$3 v = yπio_printctx_va_arg(t, _yIO_TYPE_$3);
	int err = yπio_printctx_init(t);
	if (err) return err;
	struct yπio_printfmt_s *pf = yπio_printctx_get_fmt(t);
	_yIO_CACHE_AUTO_DECL(_buf, result, length);
	err = _yIO_astrfrom$1(&result, &length, pf->precision, pf->type, v);
	if (err) return err;
	const bool negative = result[0] == '-';
	err = yπio_printctx_put_number(t, result + negative, length - negative, !negative);
	_yIO_CACHE_AUTO_FREE(_buf, result, length);
	return err;
}

~)m4_dnl;

#endif // _yIO_HAS_STDFIX_TYPES
