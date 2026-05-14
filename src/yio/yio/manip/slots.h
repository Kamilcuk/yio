/**
 * @file
 * @date 2020-06-13
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#ifndef YIO_COUNTER
#error YIO_COUNTER not defined.
{% for i in range(0, j_MAX_CUSTOM_SLOTS) %}
#line
#elif YIO_COUNTER == {{ i }}
#undef YIO_COUNTER
#define YIO_COUNTER {{ i + 1 }}
#undef YIO_PRINT_FUNC_GENERIC_SLOTS
#define YIO_PRINT_FUNC_GENERIC_SLOTS(X, XALIAS)  \
	{% for j in range(0, i + 1) %}
	X(YIO_TYPE_{{ j }}, YIO_TYPE_FUNC_{{ j }}) \
	{% endfor %}
	/* */

{% endfor %}
#line
#else
#error YIO_COUNTER is greater than {{ j_MAX_CUSTOM_SLOTS }}.
#endif

