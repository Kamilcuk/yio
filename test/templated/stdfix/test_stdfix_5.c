/**
 * @file
 * @brief Test stdfix types with various edge cases
 */
#include <yio_test_private.h>
#include <yio/private/yio_stdfix.h>

#include <stdio.h>
int main() {
	#if !YIO_HAS_STDFIX_TYPES
	return EXIT_SKIP;
	#else
	{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef YIO_STDFIX_$3
	{
		const $2 max = YIO_$3_MAX;
		
		yio_fprintf(stderr, "Testing type: $2 ($3)\n");

		// Failing Integer representations
		YIO_TEST((.rgx="[0-9a-f]+"), "{:x}", max);
		YIO_TEST((.rgx="[0-9]+"), "{:d}", max);
		YIO_TEST((.rgx="[0-9]+"), "{:u}", max);

		// Failing Large values for Accum
		{% if j_search(V.2, ".*_Accum") %}
		{
			const $2 zero = 0;
			const $2 ten = (const $2)10.0$1 < max ? (const $2)10.0$1 : zero;
			if (ten > zero) {
				YIO_TEST("10.000000", "{}", ten);
			}
		}
		{% endif %}
	}
#endif
	{% endcall %}
	#endif
}
