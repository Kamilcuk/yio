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
	_yIO_RES_AUTO_DECL(res);
	err = _yIO_astrfrom$1(&res, pf->precision, pf->type, v);
	if (err) return err;
	const char *const result = _yIO_res_begin(&res);
	const size_t length = _yIO_res_used(&res);
	const bool negative = result[0] == '-';
	err = yπio_printctx_put_number(t, result + negative, length - negative, !negative);
	_yIO_res_end(&res);
	return err;
}
{% endcall %}

#endif // _yIO_HAS_STDFIX_TYPES
