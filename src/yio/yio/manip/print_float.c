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
#include "../../private/yio_float_strfrom_naive.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_printf.h"
#include "../../private/yio_float_strfrom_ryu.h"
#include <ctype.h>

#if YYIO_CDT_PARSER
#define YYIO_FLOAT_REPR_B64  double
#endif

/* Layer 2: Representation Dispatchers */
{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_REPR_{{R}}
int YYIO_float_dispatch_{{R}}(yio_printctx_t *t, YYIO_FLOAT_REPR_{{R}} val) {
    int err = yio_printctx_init(t);
    if (err) return err;
    const char spec = t->pf.type ? t->pf.type : 'g';
    const int precision = t->pf.precision;
    YYIO_string res;
    YYIO_string_init(&res);
#if YIO_FLOAT_BACKEND_STRFROM && YYIO_has_float_strfrom_{{R}}
    err = YYIO_float_astrfrom_strfrom_{{R}}(&res, precision, spec, val);
#elif YIO_FLOAT_BACKEND_RYU && YYIO_has_float_ryu_{{R}}
    err = YYIO_float_astrfrom_ryu_{{R}}(&res, precision, spec, val);
#elif YIO_FLOAT_BACKEND_PRINTF && YYIO_has_float_printf_{{R}}
    err = YYIO_float_astrfrom_printf_{{R}}(&res, precision, spec, val);
#else
    err = YYIO_float_astrfrom_naive_{{R}}(&res, precision, spec, val);
#endif
    if (err) goto EXIT;
    const char *const result = YYIO_string_data(&res);
    const size_t length = YYIO_string_len(&res);
    const bool is_negative = (length > 0 && result[0] == '-');
    err = yio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
EXIT:
    YYIO_string_free(&res);
    return err;
}
#endif
{% endfor %}

/* Layer 1: Typed Entry Points */
{% for V in j_FLOATS %}
#line
#if YIO_HAS_FLOAT{{V.1}}
{% if V.1 in ["f", "d", "l"] %}
#line
int YYIO_print_{{V.1}}(yio_printctx_t *t) {
    const YYIO_FLOAT{{V.1}} val = yio_printctx_va_arg_promote(t, YYIO_FLOAT{{V.1}});
{% for R in V.reprs %}
#if YYIO_REPR_OF_{{V.1}}_IS_{{R}}
    return YYIO_float_dispatch_{{R}}(t, (YYIO_FLOAT_REPR_{{R}})val);
#endif
{% endfor %}
    return YIO_ERROR_ENOSYS;
}
{% else %}
#line
{% for R in V.reprs %}
#if YYIO_REPR_OF_{{V.1}}_IS_{{R}}
int YYIO_print_{{V.1}}_as_{{R}}(yio_printctx_t *t) {
    const YYIO_FLOAT{{V.1}} val = yio_printctx_va_arg_promote(t, YYIO_FLOAT{{V.1}});
    return YYIO_float_dispatch_{{R}}(t, (YYIO_FLOAT_REPR_{{R}})val);
}
#endif
{% endfor %}
{% endif %}
#endif
{% endfor %}
