/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include "private.h"
#include "../../private/yio_float.h"
#include "print_float.h"
#include "../../private/yio_string.h"
#include "../../private/yio_float_strfrom_printf.h"
#include "../../private/yio_float_strfrom_strfrom.h"
#include "../../private/yio_float_strfrom_ryu.h"
#include "../../private/yio_float_strfrom_naive.h"
#include <ctype.h>

int YIO_float_apply_alternate_form(yio_printctx_t *t, YIO_string *o) {
    const char spec = t->pf.type ? t->pf.type : 'g';
    const char speclower = YIO_tolower(spec);
    const int precision0 = (int)t->pf.precision - 1;

    char *data = YIO_string_data(o);
    size_t len = YIO_string_len(o);

    if (len == 0) return 0;
    // Skip non-numeric values (NaN, Inf)
    if (!YIO_isdigit(data[len - 1]) && data[len - 1] != '.') {
        if (YIO_ANYEQ(data[len - 1], 'n', 'N', 'i', 'I', 'f', 'F')) return 0;
    }

    char *dot = NULL;
    char *exp = NULL;
    const char exp_marker_lower = (speclower == 'a' ? 'p' : 'e');
    for (size_t i = 0; i < len; ++i) {
        if (data[i] == '.') dot = &data[i];
        if (YIO_tolower(data[i]) == exp_marker_lower) {
            exp = &data[i];
            break;
        }
    }

    if (!dot) {
        if (exp) {
            const size_t exp_off = (size_t)(exp - data);
            const size_t exp_len = len - exp_off;
            const int err = YIO_string_reserve_more(o, 1);
            if (err) return err;
            data = YIO_string_data(o);
            exp = data + exp_off;
            memmove(exp + 1, exp, exp_len);
            *exp = '.';
            len++;
            YIO_string_set_used(o, len);
            dot = data + exp_off;
            exp = data + exp_off + 1;
        } else {
            const int err = YIO_string_putc(o, '.');
            if (err) return err;
            len++;
            data = YIO_string_data(o);
            dot = data + len - 1;
        }
    }

    if (speclower == 'g') {
        const int p = precision0 < 0 ? 6 : (precision0 == 0 ? 1 : precision0);

        char *ptr = data;
        if (YIO_ANYEQ(*ptr, '+', '-')) ptr++;
        if (len - (size_t)(ptr - data) >= 2 && *ptr == '0' && YIO_ANYEQ(ptr[1], 'x', 'X')) ptr += 2;

        char *first_sig = ptr;
        while (first_sig < data + len && (*first_sig == '0' || *first_sig == '.')) first_sig++;

        if (first_sig == data + len || !YIO_isdigit(*first_sig)) {
            first_sig = ptr;
            while (first_sig < data + len && !YIO_isdigit(*first_sig)) first_sig++;
        }

        if (first_sig < data + len && YIO_isdigit(*first_sig)) {
            size_t sig_digits = 0;
            const char *q = first_sig;
            const char *const end = exp ? exp : data + len;
            while (q < end) {
                if (YIO_isdigit(*q)) sig_digits++;
                q++;
            }

            if (sig_digits < (size_t)p) {
                size_t to_add = (size_t)p - sig_digits;
                if (exp) {
                    const size_t exp_off = (size_t)(exp - data);
                    const size_t exp_len = len - exp_off;
                    const int err = YIO_string_reserve_more(o, to_add);
                    if (err) return err;
                    data = YIO_string_data(o);
                    exp = data + exp_off;
                    memmove(exp + to_add, exp, exp_len);
                    memset(exp, '0', to_add);
                    len += to_add;
                    YIO_string_set_used(o, len);
                } else {
                    while (to_add--) {
                        const int err = YIO_string_putc(o, '0');
                        if (err) return err;
                    }
                }
            }
        }
    }
    return 0;
}

static int postprocess(yio_printctx_t *t, YIO_string *o, int err) {
    if (err) goto EXIT;
    if (t->pf.hash) {
        err = YIO_float_apply_alternate_form(t, o);
        if (err) goto EXIT;
    }
    const char *const result = YIO_string_data(o);
    const size_t length = YIO_string_len(o);
    const bool is_negative = (length > 0 && result[0] == '-');
    err = yio_printctx_put_number(t, result + is_negative, length - is_negative, !is_negative);
EXIT:
    YIO_string_fini(o);
    return err;
}

/* Layer 2: Representation Dispatchers */
{% for R in j_FLOATREPRS %} #line
#ifdef YIO_FLOAT_RP_{{R.name}}
static int YIO_float_dispatch_{{R.name}}(yio_printctx_t *t, YIO_FLOAT_RP_{{R.name}} val) {
    int err = yio_printctx_init(t);
    if (err) return err;
    const char spec = t->pf.type ? t->pf.type : 'g';
    const char speclower = YIO_tolower(spec);
    if (!YIO_ANYEQ(speclower, 'a', 'e', 'f', 'g')) {
        return YIO_ERROR_FMT_INVALID;
    }
    const int precision = (int)t->pf.precision - 1;
    YIO_string res;
    YIO_string_init(&res);

#ifndef YIO_has_float_astrfrom_strfrom_{{R.name}}
#error YIO_has_float_astrfrom_strfrom_{{R.name}} is not defined
#endif
#ifndef YIO_has_float_astrfrom_ryu_{{R.name}}
#error YIO_has_float_astrfrom_ryu_{{R.name}} is not defined
#endif
#ifndef YIO_has_float_astrfrom_printf_{{R.name}}
#error YIO_has_float_astrfrom_printf_{{R.name}} is not defined
#endif

#if YIO_has_float_astrfrom_strfrom_{{R.name}}
    err = YIO_float_astrfrom_strfrom_{{R.name}}(&res, precision, spec, val);
#elif YIO_has_float_astrfrom_ryu_{{R.name}}
    err = YIO_float_astrfrom_ryu_{{R.name}}(&res, precision, spec, val);
#elif YIO_has_float_astrfrom_printf_{{R.name}}
    err = YIO_float_astrfrom_printf_{{R.name}}(&res, precision, spec, val);
#else
    err = YIO_float_astrfrom_naive_{{R.name}}(&res, precision, spec, val);
#endif
    return postprocess(t, &res, err);
}
#endif
{% endfor %} #line

/* Layer 1: Public Interface */
{% for V in j_FLOATS %} #line
#if YIO_HAS_FLOAT{{V.1}}
int YIO_print_{{V.1}}(yio_printctx_t *t) {
{% if V.type == 'float' %}
    const double val = yio_printctx_va_arg(t, double);
{% else %}
    const YIO_FLOAT{{V.1}} val = yio_printctx_va_arg_promote(t, YIO_FLOAT{{V.1}});
{% endif %}
{% for R in j_FLOATREPRS %} #line
{% if loop.first %}#if{% else %}#elif{% endif %} defined(YIO_RP_OF_{{V.1}}_IS_{{R.name}})
    return YIO_float_dispatch_{{R.name}}(t, (YIO_FLOAT_RP_{{R.name}})val);
{% endfor %} #line
#else
#error "No idea how to dispatch represetation of {{V}}"
    (void)val;
    return YIO_ERROR_ENOSYS;
#endif
}
#endif
{% endfor %}
