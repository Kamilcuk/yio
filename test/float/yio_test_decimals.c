#include <yio_test.h>
#include <float.h>
#include <string.h>
#include <stdio.h>

int main(void) {
#if YIO_HAS_FLOATd32 || YIO_HAS_FLOATd64 || YIO_HAS_FLOATd128

#if YIO_HAS_FLOATd32
#if YIO_FLOAT_BACKEND_STRFROM && !YYIO_HAS_strfromd32
    // Skip
#else
    // Special Values
    YIO_TEST("9.999999e+96", "{:e}", DEC32_MAX);
    YIO_TEST("1.000000e-95", "{:e}", DEC32_MIN);
    YIO_TEST("0.000001", "{:a}", DEC32_EPSILON);
    YIO_TEST("0.000000", "{:f}", 0.0df);
    YIO_TEST("-0.000000", "{:f}", -0.0df);
    
    YIO_TEST("inf", "{:f}", __builtin_infd32());
    YIO_TEST("-inf", "{:f}", -__builtin_infd32());
    YIO_TEST("nan", "{:g}", __builtin_nand32(""));
    YIO_TEST("INF", "{:G}", __builtin_infd32());
    YIO_TEST("-INF", "{:G}", -__builtin_infd32());
    YIO_TEST("NAN", "{:G}", __builtin_nand32(""));

    // Basic Formatting
    YIO_TEST("1.25", "{:g}", 1.25df);
    YIO_TEST("-1.25", "{:g}", -1.25df);
    YIO_TEST("1.25E-05", "{:G}", 0.0000125df);

    // Precision
    YIO_TEST("1", "{:.0f}", 1.25df);
    YIO_TEST("1.2", "{:.1f}", 1.25df);
    YIO_TEST("1.2500000000", "{:.10f}", 1.25df);

    // Padding & Alignment
    YIO_TEST("  1.25", "{:6g}", 1.25df);
    YIO_TEST("001.25", "{:06g}", 1.25df);
    YIO_TEST("1.25  ", "{:<6g}", 1.25df);
    YIO_TEST(" 1.25 ", "{:^6g}", 1.25df);
    YIO_TEST("*1.25*", "{:*^6g}", 1.25df);

    // Signs & Alternate Forms
    YIO_TEST("+1.25", "{:+g}", 1.25df);
    YIO_TEST(" 1.25", "{: g}", 1.25df);
    YIO_TEST("1.", "{:#.0f}", 1.25df);
    YIO_TEST("1.00000", "{:#g}", 1.0df);
    
    // Grouping
    YIO_TEST("1_234.567000", "{:_f}", 1234.567df);

    // Invalid formatting specifiers
    YIO_TEST_FAIL("{:x}", 1.25df);
    YIO_TEST_FAIL("{:X}", 1.25df);
#endif
#endif

#if YIO_HAS_FLOATd64
#if YIO_FLOAT_BACKEND_STRFROM && !YYIO_HAS_strfromd64
    // Skip
#else
    YIO_TEST("1.000000e+385", "{:e}", DEC64_MAX);
    YIO_TEST("1.000000e-383", "{:e}", DEC64_MIN);
    YIO_TEST("1.000000e-15", "{:e}", DEC64_EPSILON);
    YIO_TEST("0.000000", "{:f}", 0.0dd);
    YIO_TEST("-0.000000", "{:f}", -0.0dd);

    YIO_TEST("inf", "{:f}", __builtin_infd64());
    YIO_TEST("-inf", "{:f}", -__builtin_infd64());
    YIO_TEST("nan", "{:g}", __builtin_nand64(""));
    YIO_TEST("INF", "{:G}", __builtin_infd64());
    YIO_TEST("-INF", "{:G}", -__builtin_infd64());
    YIO_TEST("NAN", "{:G}", __builtin_nand64(""));

    YIO_TEST("1.25", "{:g}", 1.25dd);
    YIO_TEST("-1.25", "{:g}", -1.25dd);
    YIO_TEST("1.25E-05", "{:G}", 0.0000125dd);

    YIO_TEST("1", "{:.0f}", 1.25dd);
    YIO_TEST("1.2", "{:.1f}", 1.25dd);
    YIO_TEST("1.2500000000", "{:.10f}", 1.25dd);

    YIO_TEST("  1.25", "{:6g}", 1.25dd);
    YIO_TEST("001.25", "{:06g}", 1.25dd);
    YIO_TEST("1.25  ", "{:<6g}", 1.25dd);
    YIO_TEST(" 1.25 ", "{:^6g}", 1.25dd);
    YIO_TEST("*1.25*", "{:*^6g}", 1.25dd);

    YIO_TEST("+1.25", "{:+g}", 1.25dd);
    YIO_TEST(" 1.25", "{: g}", 1.25dd);
    YIO_TEST("1.", "{:#.0f}", 1.25dd);
    YIO_TEST("1.00000", "{:#g}", 1.0dd);

    YIO_TEST("1_234.567000", "{:_f}", 1234.567dd);

    YIO_TEST_FAIL("{:x}", 1.25dd);
    YIO_TEST_FAIL("{:X}", 1.25dd);
#endif
#endif

#if YIO_HAS_FLOATd128
#if YIO_FLOAT_BACKEND_STRFROM && !YYIO_HAS_strfromd128
    // Skip
#else
    YIO_TEST("1.000000e+6145", "{:e}", DEC128_MAX);
    YIO_TEST("1.000000e-6143", "{:e}", DEC128_MIN);
    YIO_TEST("1.000000e-33", "{:e}", DEC128_EPSILON);
    YIO_TEST("0.000000", "{:f}", 0.0dl);
    YIO_TEST("-0.000000", "{:f}", -0.0dl);

    YIO_TEST("inf", "{:f}", __builtin_infd128());
    YIO_TEST("-inf", "{:f}", -__builtin_infd128());
    YIO_TEST("nan", "{:g}", __builtin_nand128(""));
    YIO_TEST("INF", "{:G}", __builtin_infd128());
    YIO_TEST("-INF", "{:G}", -__builtin_infd128());
    YIO_TEST("NAN", "{:G}", __builtin_nand128(""));

    YIO_TEST("1.25", "{:g}", 1.25dl);
    YIO_TEST("-1.25", "{:g}", -1.25dl);
    YIO_TEST("1.25E-05", "{:G}", 0.0000125dl);

    YIO_TEST("1", "{:.0f}", 1.25dl);
    YIO_TEST("1.2", "{:.1f}", 1.25dl);
    YIO_TEST("1.2500000000", "{:.10f}", 1.25dl);

    YIO_TEST("  1.25", "{:6g}", 1.25dl);
    YIO_TEST("001.25", "{:06g}", 1.25dl);
    YIO_TEST("1.25  ", "{:<6g}", 1.25dl);
    YIO_TEST(" 1.25 ", "{:^6g}", 1.25dl);
    YIO_TEST("*1.25*", "{:*^6g}", 1.25dl);

    YIO_TEST("+1.25", "{:+g}", 1.25dl);
    YIO_TEST(" 1.25", "{: g}", 1.25dl);
    YIO_TEST("1.", "{:#.0f}", 1.25dl);
    YIO_TEST("1.00000", "{:#g}", 1.0dl);

    YIO_TEST("1_234.567000", "{:_f}", 1234.567dl);

    YIO_TEST_FAIL("{:x}", 1.25dl);
    YIO_TEST_FAIL("{:X}", 1.25dl);
#endif
#endif

#else
    fprintf(stderr, "decimal is disabled\n");
    return 77;
#endif

    return 0;
}
