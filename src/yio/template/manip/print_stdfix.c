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

#if _yIO_HAS_STDFIX_TYPES

#include "../../private/yio_stdfix.h"
#include "../../private/yio_stdfix_strfrom.h"

{% from "yio/private/yio_stdfix.h" import j_STDFIX %}

{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
int _yΩIO_print_$3(yπio_printctx_t *t) {
	const _yIO_STDFIX_$3 v = yπio_printctx_va_arg(t, _yIO_STDFIX_$3);
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
{% endcall %}

#endif // _yIO_HAS_STDFIX_TYPES
