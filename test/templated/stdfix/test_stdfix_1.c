/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
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
		const $2 a = 0.125;
		yio_fprint(stderr, "{}\n", "$2");
		YIO_TEST((.rgx="[0-9a-f]+"), "{:x}", a);
		YIO_TEST((.rgx="0\\.[0-9][0-9][0-9][0-9][0-9][0-9]"), "{}", a);
		YIO_TEST((.rgx="0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*"), "{:a}", a);
	}
#endif
	{% endcall %}
	#endif
}
