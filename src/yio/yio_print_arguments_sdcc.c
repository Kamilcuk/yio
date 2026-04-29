/**
 * @file
 * @date 2026-04-28
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

#include "yio_print_arguments_sdcc.h"

#ifdef __SDCC

const void * yyio_sdcc_args[YIO_SDCC_MAX_ARGS];

{% for I in j_one_to_n(2, j_MLVLS) %}
const void * yyio_init_args_{{ I }}(
{%- for J in j_one_to_n(2, I) -%}
const void * _{{ J }}{% if not loop.last %}, {% endif %}
{%- endfor -%}
) {
{%- for J in j_one_to_n(2, I) %}
	yyio_sdcc_args[{{ loop.index0 }}] = _{{ J }};
{%- endfor %}
	yyio_sdcc_args[{{ I - 1 }}] = 0;
	return yyio_sdcc_args;
}
{% endfor %}

#endif
