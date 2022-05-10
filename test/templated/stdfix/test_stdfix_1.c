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

{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}
#line

int main() {
	{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
		// There is a problem with long long operations as of now
		{% if not j_match(V.1, ".*long.*long.*") %}
#line
#ifdef _yIO_STDFIX_$3
	{
		const $2 a = 0.125;
		yfprintf(stderr, "{}\n", "$2");
		YΩIO_TEST("[0-9a-f]+", "{:x}", a);
		YΩIO_TEST("0\\.[0-9][0-9][0-9][0-9][0-9][0-9]", "{}", a);
		YΩIO_TEST("0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*", "{:a}", a);
	}
#endif
		{% endif %}
	{% endcall %}
}
