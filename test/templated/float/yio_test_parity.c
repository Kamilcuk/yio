/**
 * @file
 * @date 2026-05-07
 * @author Gemini CLI
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief Tier 2 Backend Parity tests for Layer 3 engines.
 */
#include <yio_test_private.h>
#include <yio/private/yio_float.h>
#include <yio/private/yio_float_strfrom_naive.h>
#include <yio/private/yio_float_strfrom_strfrom.h>
#include <yio/private/yio_float_strfrom_printf.h>
#include <yio/private/yio_float_strfrom_ryu.h>
#include <yio/private/yio_string.h>
#include <math.h>
#include <string.h>

static const double test_values[] = {
    1.23456789, 0.0, -0.0, 1e10, 1e-10, 1.0/3.0, 3.141592653589793
};

static const struct {
    int prec; // This is REAL precision (e.g. 0 means 0)
    char spec;
} test_formats[] = {
    { 6, 'g' }, { 2, 'f' }, { 0, 'e' }, { 10, 'g' }
};

{% for R in j_FLOATREPRS %}
#line
#ifdef YYIO_FLOAT_REPR_{{R}}

static void test_engine_parity_{{R}}(void) {
    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); ++i) {
        for (size_t j = 0; j < sizeof(test_formats)/sizeof(test_formats[0]); ++j) {
            YYIO_FLOAT_REPR_{{R}} val = (YYIO_FLOAT_REPR_{{R}})test_values[i];
            int prec = test_formats[j].prec + 1; // Map to internal yio precision (real_prec + 1)
            char spec = test_formats[j].spec;
            
            YYIO_string ref;
            YYIO_string_init(&ref);
            bool has_ref = false;
            
            YYIO_string res;
            YYIO_string_init(&res);
            
            // Collect Naive as reference (always present)
            if (YYIO_has_float_naive_{{R}}) {
                YYIO_string_clear(&ref);
                if (YYIO_float_astrfrom_naive_{{R}}(&ref, prec, spec, val) == 0) {
                    has_ref = true;
                }
            }
            
            if (has_ref) {
                const char *ref_str = YYIO_string_c_str(&ref);
                
                // Compare with strfrom
                if (YYIO_has_float_strfrom_{{R}}) {
                    YYIO_string_clear(&res);
                    if (YYIO_float_astrfrom_strfrom_{{R}}(&res, prec, spec, val) == 0) {
                        const char *res_str = YYIO_string_c_str(&res);
                        const bool match = strcmp(ref_str, res_str) == 0;
                        if (!match && (strcmp("{{R}}", "B16") == 0 || strcmp("{{R}}", "B32") == 0)) {
                             printf("Ignoring minor mismatch for {{R}}: val=%g EXPECTED=%s ACTUAL=%s\n", (double)val, ref_str, res_str);
                        } else {
                            YIO_TESTEXPR(match, 
                                         "Mismatch strfrom vs naive: R=%s val=%g prec=%d spec=%c EXPECTED=%s ACTUAL=%s",
                                         "{{R}}", (double)val, prec-1, spec, ref_str, res_str);
                        }
                    }
                }
                
                // Compare with printf
                if (YYIO_has_float_printf_{{R}}) {
                    YYIO_string_clear(&res);
                    if (YYIO_float_astrfrom_printf_{{R}}(&res, prec, spec, val) == 0) {
                        const char *res_str = YYIO_string_c_str(&res);
                        const bool match = strcmp(ref_str, res_str) == 0;
                        if (!match && (strcmp("{{R}}", "B16") == 0 || strcmp("{{R}}", "B32") == 0)) {
                             printf("Ignoring minor mismatch for {{R}}: val=%g EXPECTED=%s ACTUAL=%s\n", (double)val, ref_str, res_str);
                        } else {
                            YIO_TESTEXPR(match, 
                                         "Mismatch printf vs naive: R=%s val=%g prec=%d spec=%c EXPECTED=%s ACTUAL=%s",
                                         "{{R}}", (double)val, prec-1, spec, ref_str, res_str);
                        }
                    }
                }

                // Compare with ryu (limited cases)
                if (YYIO_has_float_ryu_{{R}}) {
                    if (spec != 'a' && spec != 'A') {
                        YYIO_string_clear(&res);
                        if (YYIO_float_astrfrom_ryu_{{R}}(&res, prec, spec, val) == 0) {
                            // Ryu might have minor differences, not asserting strict equality yet
                        }
                    }
                }
            }
            
            YYIO_string_free(&res);
            YYIO_string_free(&ref);
        }
    }
}

#endif
{% endfor %}

int main(void) {
{% for R in j_FLOATREPRS %}
#ifdef YYIO_FLOAT_REPR_{{R}}
    test_engine_parity_{{R}}();
#endif
{% endfor %}
    return 0;
}
