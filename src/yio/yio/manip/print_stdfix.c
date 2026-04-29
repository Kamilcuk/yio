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

#if YYIO_HAS_STDFIX_TYPES

#include "../../private/yio_stdfix.h"
#include "../../private/yio_stdfix_strfrom.h"

{% from "yio/private/yio_stdfix.h" import j_STDFIX %}

{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
int YYIO_print_$3(yio_printctx_t *t) {
	const YYIO_STDFIX_$3 v = yio_printctx_va_arg(t, YYIO_STDFIX_$3);
	int err = yio_printctx_init(t);
	if (err) return err;
	struct yio_printfmt_s *pf = yio_printctx_get_fmt(t);
	YYIO_string res = {0};
	err = YYIO_astrfrom$1(&res, pf, v);
	if (err) return err;
	const char *const result = YYIO_string_data(&res);
	const size_t length = YYIO_string_len(&res);
	const bool negative = result[0] == '-';
	err = yio_printctx_put_number(t, result + negative, length - negative, !negative);
	YYIO_string_free(&res);
	return err;
}
{% endcall %}

#endif // YYIO_HAS_STDFIX_TYPES
