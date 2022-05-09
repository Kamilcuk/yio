/**
 * @file
 * @date 2020-sie-03
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#include <yio_test.h>

{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}
#line

int main() {
	{% call j_FOREACHAPPLY(j_STDFIX) %}
#line
#ifdef _yIO_STDFIX_$3
	{
		const $2 a = 0.123456789;
		YΩIO_TEST("[^:]+: [^ ]+ [0-9a-f]+ 0\\.[0-9][0-9][0-9][0-9][0-9][0-9] 0x[0-9a-f].?[0-9a-f]*p[-+][0-9]*",
				"{:25}: {} {:x} {} {:a}", "$2", (float)a, a, a, a);
	}
#endif
	{% endcall %}
}
