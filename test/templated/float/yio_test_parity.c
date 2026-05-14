/**
 * @file
 * @date 2020-08-16
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test_private.h>
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_naive.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_printf.h>
#include <yio/private/yio_float_strfrom_ryu.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct YIO_testparams {
    int prec;
    char spec;
};

static const struct YIO_testparams testparams[] = {
    { 6, 'g' }, { 2, 'f' }, { 0, 'e' }, { 10, 'g' }
};

{% for R in j_FLOATREPRS %}
#line
#ifdef YIO_FLOAT_RP_{{R.name}}

static void test_engine_parity_{{R.name}}(void) {
#ifndef YIO_has_float_astrfrom_strfrom_{{R.name}}
#error YIO_has_float_astrfrom_strfrom_{{R.name}} is not defined
#endif
#ifndef YIO_has_float_astrfrom_ryu_{{R.name}}
#error YIO_has_float_astrfrom_ryu_{{R.name}} is not defined
#endif
#ifndef YIO_has_float_astrfrom_printf_{{R.name}}
#error YIO_has_float_astrfrom_printf_{{R.name}} is not defined
#endif
    YIO_string res; YIO_string_init(&res);
    YIO_string ref; YIO_string_init(&ref);
    
    // Test values from the test list
#if 0
    // Skip large test list for now, it's very slow
#endif

    const YIO_FLOAT_RP_{{R.name}} test_vals[] = {
        0.0, 1.23456, 123456.789, 0.000123, -1.23, 1e10, 1e-10
    };

    for (size_t i = 0; i < sizeof(test_vals)/sizeof(test_vals[0]); ++i) {
        YIO_FLOAT_RP_{{R.name}} val = (YIO_FLOAT_RP_{{R.name}})test_vals[i];
        for (size_t j = 0; j < sizeof(testparams)/sizeof(testparams[0]); ++j) {
            int prec = testparams[j].prec;
            char spec = testparams[j].spec;
            
            YIO_string_clear(&ref);
            YIO_string_clear(&res);
            
            if (YIO_has_float_astrfrom_naive_{{R.name}}) {
                if (YIO_float_astrfrom_naive_{{R.name}}(&ref, prec, spec, val) == 0) {
                    
#if YIO_has_float_astrfrom_strfrom_{{R.name}}
                    YIO_string_clear(&res);
                    if (YIO_float_astrfrom_strfrom_{{R.name}}(&res, prec, spec, val) == 0) {
                        if (!YIO_test_string_float_equal(&ref, &res, spec)) {
                            printf("Mismatch {{R.name}} strfrom: val=%g prec=%d spec=%c NAIVE=%s STRFROM=%s\n",
                                   (double)val, prec, spec, YIO_string_c_str(&ref), YIO_string_c_str(&res));
                        }
                    }
#endif

#if YIO_has_float_astrfrom_printf_{{R.name}}
                    YIO_string_clear(&res);
                    if (YIO_float_astrfrom_printf_{{R.name}}(&res, prec, spec, val) == 0) {
                        if (!YIO_test_string_float_equal(&ref, &res, spec)) {
                            printf("Mismatch {{R.name}} printf: val=%g prec=%d spec=%c NAIVE=%s PRINTF=%s\n",
                                   (double)val, prec, spec, YIO_string_c_str(&ref), YIO_string_c_str(&res));
                        }
                    }
#endif

#if YIO_has_float_astrfrom_ryu_{{R.name}}
                    YIO_string_clear(&res);
                    if (YIO_float_astrfrom_ryu_{{R.name}}(&res, prec, spec, val) == 0) {
                        if (!YIO_test_string_float_equal(&ref, &res, spec)) {
                            printf("Mismatch {{R.name}} ryu: val=%g prec=%d spec=%c NAIVE=%s RYU=%s\n",
                                   (double)val, prec, spec, YIO_string_c_str(&ref), YIO_string_c_str(&res));
                        }
                    }
#endif
                }
            }
        }
    }
    
    YIO_string_fini(&res);
    YIO_string_fini(&ref);
}
#endif
{% endfor %}

int main(void) {
{% for R in j_FLOATREPRS %}
#ifdef YIO_FLOAT_RP_{{R.name}}
    test_engine_parity_{{R.name}}();
#endif
{% endfor %}
    return 0;
}
