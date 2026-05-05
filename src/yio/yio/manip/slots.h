/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#ifndef YYIO_COUNTER
#error YYIO_COUNTER not defined.
{% for i in range(0, j_MAX_CUSTOM_SLOTS) %}
#line
#elif YYIO_COUNTER == {{ i }}
#undef YYIO_COUNTER
#define YYIO_COUNTER {{ i + 1 }}
#undef YYIO_PRINT_FUNC_GENERIC_SLOTS
#define YYIO_PRINT_FUNC_GENERIC_SLOTS()  \
	{% for j in range(0, i + 1) %}
	YYIO_OVERLOAD_TYPE_FUNC(YYIO_TYPE_{{ j }}, YYIO_TYPE_FUNC_{{ j }}) \
	{% endfor %}
	/* */
{% endfor %}
#line
#else
#error YYIO_COUNTER is greater than {{ j_MAX_CUSTOM_SLOTS }}.
#endif

