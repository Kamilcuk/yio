/**
 * @file
 * @date 2026-05-08
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief Unit test for YIO_float_apply_alternate_form
 */
#include "yio_test_private.h"
#include "yio/yio/manip/private.h"
#include <string.h>
#include <stdio.h>

static void test_case(const char *input, char spec, int precision, const char *expected) {
    yio_printctx_t t = {0};
    t.pf.type = spec;
    t.pf.precision = (uint16_t)(precision + 1);
    t.pf.hash = true;

    YIO_string o;
    YIO_string_init(&o);
    int err = YIO_string_putsn(&o, input, strlen(input));
    YIO_TESTEXPR(err == 0, "YIO_string_putsn failed for '%s' (spec=%c, prec=%d)", input, spec, precision);

    err = YIO_float_apply_alternate_form(&t, &o);
    YIO_TESTEXPR(err == 0, "YIO_float_apply_alternate_form failed for '%s' (spec=%c, prec=%d)", input, spec, precision);

    const char *actual = YIO_string_c_str(&o);
    YIO_TESTEXPR(actual != NULL && strcmp(actual, expected) == 0,
                 "Mismatch for '%s' (spec=%c, prec=%d): EXPECTED='%s' ACTUAL='%s'",
                 input, spec, precision, expected, actual ? actual : "(null)");

    YIO_string_fini(&o);
}

int main(void) {
    // Specifier 'f' / 'F'
    test_case("123", 'f', 0, "123.");
    test_case("+123", 'f', 0, "+123.");
    test_case("-123", 'f', 0, "-123.");
    test_case("123", 'F', 0, "123.");
    test_case("123.456", 'f', 6, "123.456");

    // Specifier 'e' / 'E'
    test_case("1e+02", 'e', 0, "1.e+02");
    test_case("+1e+02", 'e', 0, "+1.e+02");
    test_case("-1e+02", 'e', 0, "-1.e+02");
    test_case("1E+02", 'E', 0, "1.E+02");
    test_case("1.23e+02", 'e', 6, "1.23e+02");

    // Specifier 'g' / 'G'
    test_case("1", 'g', 0, "1.");
    test_case("+1", 'g', 0, "+1.");
    test_case("-1", 'g', 0, "-1.");
    test_case("1", 'G', 0, "1.");
    test_case("1", 'g', 3, "1.00");
    test_case("1.2", 'g', 3, "1.20");
    test_case("+1.2", 'g', 3, "+1.20");
    test_case("123.456", 'g', 6, "123.456");
    test_case("123.456", 'g', 8, "123.45600");
    test_case("1.2e+02", 'g', 3, "1.20e+02");
    test_case("1E+02", 'G', 0, "1.E+02");
    test_case("0", 'g', 0, "0.");
    test_case("0.001", 'g', 3, "0.00100");

    // Specifier 'a' / 'A'
    test_case("0x1p+0", 'a', 0, "0x1.p+0");
    test_case("+0x1p+0", 'a', 0, "+0x1.p+0");
    test_case("-0x1p+0", 'a', 0, "-0x1.p+0");
    test_case("0X1P+0", 'A', 0, "0X1.P+0");
    test_case("0x1.2p+0", 'a', 6, "0x1.2p+0");
    test_case("0x1.23p+0", 'a', 0, "0x1.23p+0");
    test_case("0x1.23456789abcdefp+0", 'a', 0, "0x1.23456789abcdefp+0");
    test_case("0xe.aaddd0d2ac39f9300000p-5", 'a', 20, "0xe.aaddd0d2ac39f9300000p-5");
    test_case("0XE.AADDD0D2AC39F9300000P-5", 'A', 20, "0XE.AADDD0D2AC39F9300000P-5");

    // Non-numeric
    test_case("inf", 'f', 0, "inf");
    test_case("+inf", 'f', 0, "+inf");
    test_case("-inf", 'f', 0, "-inf");
    test_case("INF", 'F', 0, "INF");
    test_case("+INF", 'F', 0, "+INF");
    test_case("-INF", 'F', 0, "-INF");
    test_case("nan", 'g', 6, "nan");
    test_case("+nan", 'g', 0, "+nan");
    test_case("-nan", 'g', 0, "-nan");
    test_case("NAN", 'G', 0, "NAN");
    test_case("+NAN", 'G', 0, "+NAN");
    test_case("-NAN", 'G', 0, "-NAN");

    return 0;
}
