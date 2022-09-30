/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#ifndef YYΩIO_COUNTER
#error YYΩIO_COUNTER not defined.
{% set max = 100 %}
{% for i in range(0, max) %}
#line
#elif YYΩIO_COUNTER == {{ i }}
#undef YYΩIO_COUNTER
#define YYΩIO_COUNTER {{ i + 1 }}
#undef YYΩIO_PRINT_FUNC_GENERIC_SLOTS
#define YYΩIO_PRINT_FUNC_GENERIC_SLOTS()  \
	{% for j in range(0, i) %}
	YYΩIO_TYPE_{{ j }}: YYΩIO_TYPE_FUNC_{{ j }},  \
	{% endfor %}
	/* */
{% endfor %}
#line
#else
#error YYΩIO_COUNTER is greater than {{ max }}.
#endif

