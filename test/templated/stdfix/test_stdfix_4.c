/**
 * @file
 * @brief Test stdfix types with various edge cases
 */
#include <yio_test_private.h>
#include <yio/private/yio_stdfix.h>

int main() {
	{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YYIO_STDFIX_$3
	{
		const $2 max = YYIO_$3_MAX;
		const $2 min = YYIO_$3_MIN;
		const $2 eps = YYIO_$3_EPSILON;
		const $2 zero = 0;
		
		yio_fprintf(stderr, "Testing type: $2 ($3)\n");

		// Zero and basic formatting
		YIO_TEST("0.000000", "{}", zero);
		YIO_TEST("0", "{:.0f}", zero);
		YIO_TEST("0.", "{:#.0f}", zero);
		YIO_TEST(" 0.000000", "{: f}", zero);
		YIO_TEST("+0.000000", "{:+f}", zero);

		// Max and Min values
		YIO_TEST((.rgx="[0-9]+\\.[0-9]+"), "{}", max);
		YIO_TEST((.rgx="-?[0-9]+\\.[0-9]+"), "{}", min);
		
		// Epsilon (smallest positive value)
		YIO_TEST((.rgx="0\\.0*[0-9]+"), "{}", eps);

		// Negative numbers (for signed types)
		{% if not j_search(V.2, ".*unsigned.*") %}
		{
			const $2 half = 0.5$1;
			const $2 n_half = -half;
			YIO_TEST("-0.500000", "{}", n_half);
			YIO_TEST("-0.5", "{:g}", n_half);
			YIO_TEST("-0.50", "{:.2f}", n_half);
			
			const $2 one = {% if j_search(V.2, ".*_Accum") %} 1.0$1 {% else %} max {% endif %};
			if (one > 0) {
				YIO_TEST((.rgx="-?[0-9]+\\.[0-9]+"), "{}", (const $2)-one);
			}
		}
		{% endif %}

		// Rounding tests
		{
			// 0.125 is 1/8, exactly representable in binary fixed point
			const $2 val = 0.125$1;
			YIO_TEST("0.125000", "{:f}", val);
			YIO_TEST("0.13", "{:.2f}", val); // rounds up
			YIO_TEST("0.1", "{:.1f}", val);  // rounds down
			
			// Test rounding near 0.5 of the last digit
			// 0.375 is 3/8
			const $2 v2 = 0.375$1;
			YIO_TEST("0.38", "{:.2f}", v2);
			YIO_TEST("0.4", "{:.1f}", v2);
		}

		// Width and alignment
		YIO_TEST((.rgx=" *[0-9]+\\.[0-9]+"), "{:20f}", max);
		YIO_TEST((.rgx="[0-9]+\\.[0-9]+ *"), "{:<20f}", max);
		YIO_TEST((.rgx=" *[0-9]+\\.[0-9]+ *"), "{:^20f}", max);

		// Hexadecimal/Binary representation if supported
		YIO_TEST((.rgx="0x[0-9a-f].?[0-9a-f]*p[-+][0-9]+"), "{:a}", max);
		YIO_TEST((.rgx="[0-9a-f]+"), "{:x}", max);

		// Alternate form (always show decimal point)
		YIO_TEST((.rgx="[0-9]+\\."), "{:#.0f}", max);
		
		// Large values for Accum
		{% if j_search(V.2, ".*_Accum") %}
		{
			const $2 ten = (const $2)10.0$1 < max ? (const $2)10.0$1 : zero;
			if (ten > zero) {
				YIO_TEST("10.000000", "{}", ten);
				YIO_TEST("10", "{:.0f}", ten);
			}
		}
		{% endif %}

		// Small values near epsilon
		{
			const $2 small = eps * 3;
			if (small > eps) {
				YIO_TEST((.rgx="0\\.0*[0-9]+"), "{}", small);
			}
		}
	}
#endif
	{% endcall %}
}
